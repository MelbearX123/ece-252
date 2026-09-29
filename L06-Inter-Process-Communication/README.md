# Lecture 6 — Inter-Process Communication (IPC)

## 1. What IPC Is

**Inter-process communication** is how two or more processes coordinate or exchange data; the OS provides the facilities. Motivations: splitting a big task into subtasks, letting multiple users work on the same data, modular system design. (Every email you send involves IPC.)

Vocabulary:

- **Message** — the data transferred.
- **Sender** / **receiver** — the processes on each end.
- The two sides must **agree on the format** (e.g. "it's XML with these fields"). That agreement happens *outside* the OS — a meeting, published documentation, etc.

### Synchronous vs. asynchronous

| | Meaning |
|-|---------|
| **Synchronous send** | Sender **blocks** until the message is received. |
| **Asynchronous send** | Sender posts the message and **continues**. |
| **Synchronous receive** | Receiver **blocks** until a message arrives. |
| **Asynchronous receive** | Receiver checks; if nothing's there it's told so and **continues**. |

The four combinations:

1. **Sync send / sync receive** — both wait for each other (a "rendezvous").
2. **Sync send / async receive** — very uncommon.
3. **Async send / sync receive** — **the most common**: the receiver usually needs the message to continue.
4. **Async send / async receive** — neither waits.

With asynchronous receive, the receiver often sends back an **acknowledgement** message (then the roles reverse).

### Producer–consumer

The general IPC paradigm: a **producer** creates information that a **consumer** later uses (database produces records → shell displays them; web server produces pages → browser consumes them). Applies to client–server designs generally.

### Three approaches

1. **File system**
2. **Message passing**
3. **Shared memory** (Lecture 9)

A system can implement all three; none is best for every situation.

## 2. IPC via the File System

The producer writes a file in an agreed-upon location; the consumer reads it. The OS is still involved (creation, permissions).

- **Pros**: messages are **persistent** (survive a reboot); works even when sender and receiver know nothing about each other.
- **Con**: with one shared file you need **coordination** so one process doesn't overwrite another's data.
- **Workaround**: use **many files with unique names**, e.g. producer drops XML files into an `import/` directory, consumer scans the directory, imports and deletes each file. One writes, one reads, names are distinct → no overwrites, even if a second message arrives before the first is picked up.

## 3. Message Passing

The sender gives a message to the **OS** and asks it to deliver it to a recipient. Two basic operations: **send** and **receive**. Messages may be fixed or variable size. In the simplest case you address the recipient **directly** by its process ID.

### 3.1 Signals as messages

A signal is an interrupt with an ID and **no payload** — like a pager that only shows who paged you, or a fire alarm. That's enough when both sides have agreed in advance what the signal means.

Header: `<signal.h>` — use names like `SIGINT`, not raw numbers (numbers differ between systems).

#### Sending

```c
int kill( pid_t pid, int signo );   /* send signo to pid */
int raise( int signo );             /* send signo to yourself */
```

Both return 0 on success, -1 on failure (e.g. no such process).

You need the recipient's **pid**. A common convention: a service writes its pid to a known file, e.g. MySQL writes it to `/var/run/mysqld/mysqld.pid`.

`kill`'s `pid` argument has special cases:

| `pid` | Sends to |
|-------|----------|
| `> 0` | That specific process. |
| `== 0` | Every process in the **caller's process group**. |
| `== -1` | Every process the caller has permission to signal (except `init` and itself) — a broadcast. |
| `< -1` | Every process in process group `abs(pid)`. |

**The null signal**: `kill(pid, 0)` sends nothing but checks whether the process exists; if not, it fails with `errno == ESRCH`. Limited usefulness because (1) the process could exit right after you check, and (2) pids are **reused**, so it may now be a different process.

#### Delivery, pending, and blocking

- A signal is **generated**, then later **delivered**. In between it is **pending**. Delivery happens at the first opportunity — immediately if the recipient is currently running, otherwise when it next runs.
- A process can **block** signals (except `SIGKILL` and `SIGSTOP`). A blocked signal stays **pending** until unblocked. Blocking is supposed to be temporary.
- If the same signal is sent **several times while blocked**, it may be delivered **only once** — signals aren't a reliable counter.

#### Disposition — what happens on delivery

Three choices: **(1) ignore** it, **(2) run a signal handler**, **(3) run the default action** (used to undo an earlier change).

Registering a handler:

```c
void (*signal( int signo, void (*handler)(int) ))(int);   /* scary declaration */
```

Read it as: `signal(signo, handler)` — "for signal X, run function `handler`". It returns the previous handler (so you could restore it). In practice:

```c
void sig_handler( int signal_num ) {
    /* Handle the signal in some way */
}
...
signal( SIGINT, sig_handler );
```

- The handler must return `void` and take one `int`: the number of the signal received — so one handler can serve several signals and tell them apart.
- `SIG_IGN` and `SIG_DFL` can be passed instead of a function to ignore / restore the default.

Flow: the program is running normally → signal is delivered → the kernel pauses the program **between two instructions** and calls the handler → handler returns → the program resumes where it was.

#### Handler restrictions: reentrancy

Because the handler can run *between any two instructions*, it must not break whatever it interrupted. If the program was in the middle of `malloc` and the handler also calls `malloc`, the allocator's internal state can be corrupted.

Only call **reentrant** functions — ones that can be interrupted, have another complete call run, and then resume correctly. Look for the designation **async-signal-safe** (`man 7 signal-safety`).

**Avoid in handlers**: `printf`, `malloc`, `free`, and **anything that could block** (e.g. reading a file). Safe examples: `write`, `_exit`, setting a `volatile sig_atomic_t` flag.

#### Blocking signals: `sigprocmask` and signal sets

```c
int sigprocmask( int how, const sigset_t *set, sigset_t *old_set );
```

| `how` | Effect |
|-------|--------|
| `SIG_BLOCK` | Add the signals in `set` to the blocked mask. |
| `SIG_UNBLOCK` | Remove the signals in `set` from the mask. |
| `SIG_SETMASK` | Replace the mask with `set`. |

If `old_set` is non-NULL it receives the mask **before** the change (so you can restore it later).

Helpers to build a `sigset_t`:

```c
int sigemptyset( sigset_t *set );              /* start empty */
int sigaddset( sigset_t *set, int signal );    /* add one */
int sigfillset( sigset_t *set );               /* add ALL */
int sigdelset( sigset_t *set, int signal );    /* remove one */
int sigismember( sigset_t *set, int signal );  /* 1 if in set, else 0 */
```

Pattern — block Ctrl-C for a critical section, then restore:

```c
sigset_t set, previous;
sigemptyset( &set );
sigaddset( &set, SIGINT );
sigprocmask( SIG_BLOCK, &set, &previous );
/* SIGINT is blocked in this section (any Ctrl-C stays pending) */
sigprocmask( SIG_SETMASK, &previous, NULL );   /* pending SIGINT delivered now */
```

(`sigaction` is the more powerful, recommended alternative to `signal`, but out of scope.)

#### Waiting for a signal: `pause`

```c
int pause( void );
```

Suspends the process until a signal handler runs; always returns -1.

### 3.2 Message queues (System V)

Signals have two limitations: you need the recipient's pid, and there's no payload. Message queues fix both via **indirect communication**: messages go to a **mailbox (queue) owned by the OS**, independent of any process. Process A sends into the queue; process B receives from it.

Queue limits (number of queues, messages per queue, message size) depend on the OS. When a queue is full the design options are: **block** until space frees up, **overwrite** older messages, or **discard** the new message.

#### Step 1 — get a key

A key is an integer that identifies the IPC structure. Two ways:

```c
key_t ftok( char *pathname, int proj );   /* "file to key", <sys/ipc.h> */
```

Generates a key from an **existing file's** inode plus `proj` (so one file can yield several keys). Two unrelated processes calling `ftok` with the same arguments get the same key. Tiny chance of collision — acceptable.

Or use **`IPC_PRIVATE`** — guaranteed unique; used when the communicating processes are **parent and child** (the child inherits the queue id across `fork`).

#### Step 2 — get the queue

```c
int msgget( key_t key, int flag );
```

- `flag` = UNIX permissions (e.g. `0600`, `0666`) OR'ed with options:
  - `IPC_CREAT` — create if it doesn't exist.
  - `IPC_CREAT | IPC_EXCL` — create, but **fail if it already exists**.
- Returns the **queue id**.

#### Step 3 — the message format

```c
struct msgbuf {
    long mtype;      /* must be > 0 */
    char mtext[1];   /* placeholder: really "any data you want" */
};
```

The only rule: the struct must **start with a `long mtype`**; anything can follow:

```c
struct pirate_msgbuf {
    long mtype;
    struct pirate_info {
        char name[30];
        char ship_type;
        int notoriety;
        int cruelty;
        int booty_value;
    } info;
};
```

#### Step 4 — send and receive

```c
int msgsnd( int msqid, const void *ptr, size_t nbytes, int flag );
ssize_t msgrcv( int msqid, void *ptr, size_t nbytes, long type, int flag );
```

- `nbytes` = size of the payload **excluding** the `mtype` field (e.g. `sizeof(struct msgbuf) - sizeof(long)`).
- `flag` = `0` to **block** (send blocks if the queue is full; receive blocks if there's no matching message), or `IPC_NOWAIT` to fail immediately instead (errno `EAGAIN` / `ENOMSG`).
- `msgrcv` `type` selects which message:

| `type` | Returns |
|--------|---------|
| `== 0` | The first message in the queue (any type) — plain FIFO. |
| `> 0` | The first message whose `mtype == type`. |
| `< 0` | The first message with the **lowest** `mtype` that is `<= abs(type)` — a priority queue. |

#### Step 5 — clean up

```c
int msgctl( int msqid, int command, struct msqid_ds *buf );
msgctl( msqid, IPC_RMID, NULL );     /* delete queue + any remaining messages NOW */
```

⚠️ Queues are owned by the OS and **outlive your process**. If you forget `IPC_RMID` they leak until reboot. Inspect with `ipcs -q`, remove with `ipcrm -q <id>`.

#### Full example: parent → child

```c
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>

struct msg {
    long mtype;
    int data;
};

int main( int argc, char **argv ) {
    int msgqid = msgget( IPC_PRIVATE, 0666 | IPC_CREAT );

    int pid = fork();
    if ( pid > 0 ) {                         /* Parent */
        struct msg m;
        m.mtype = 42;
        m.data = 252;
        msgsnd( msgqid, &m, sizeof( int ), 0 );
    } else if ( pid == 0 ) {                 /* Child */
        struct msg m2;
        msgrcv( msgqid, &m2, sizeof( int ), 42, 0 );   /* blocks until type 42 arrives */
        printf( "Received %d!\n", m2.data );
        msgctl( msgqid, IPC_RMID, NULL );
    }
    return 0;
}
```

Note this is **async send / sync receive**: the parent sends and exits immediately; the child blocks until the message arrives.

---

## New Functions Introduced

| Function | Header | Signature | Purpose |
|----------|--------|-----------|---------|
| `kill` | `<signal.h>` | `int kill( pid_t pid, int signo )` | Send a signal to a process / group / everyone. `signo = 0` = existence check. |
| `raise` | `<signal.h>` | `int raise( int signo )` | Send a signal to yourself. |
| `signal` | `<signal.h>` | `void (*signal( int signo, void (*handler)(int) ))(int)` | Set the disposition: handler function, `SIG_IGN`, or `SIG_DFL`. Returns previous handler. |
| `sigprocmask` | `<signal.h>` | `int sigprocmask( int how, const sigset_t *set, sigset_t *old )` | Block/unblock/set the signal mask (`SIG_BLOCK`, `SIG_UNBLOCK`, `SIG_SETMASK`). |
| `sigemptyset` | `<signal.h>` | `int sigemptyset( sigset_t *set )` | Initialize an empty set. |
| `sigaddset` | `<signal.h>` | `int sigaddset( sigset_t *set, int sig )` | Add a signal to a set. |
| `sigfillset` | `<signal.h>` | `int sigfillset( sigset_t *set )` | Add all signals to a set. |
| `sigdelset` | `<signal.h>` | `int sigdelset( sigset_t *set, int sig )` | Remove a signal from a set. |
| `sigismember` | `<signal.h>` | `int sigismember( const sigset_t *set, int sig )` | Test membership (1/0). |
| `pause` | `<unistd.h>` | `int pause( void )` | Sleep until a signal handler runs. |
| `ftok` | `<sys/ipc.h>` | `key_t ftok( const char *path, int proj )` | Make an IPC key from an existing file + project id. |
| `msgget` | `<sys/msg.h>` | `int msgget( key_t key, int flag )` | Create/open a message queue; returns queue id. |
| `msgsnd` | `<sys/msg.h>` | `int msgsnd( int id, const void *msg, size_t nbytes, int flag )` | Send a message (payload size excludes `mtype`). |
| `msgrcv` | `<sys/msg.h>` | `ssize_t msgrcv( int id, void *msg, size_t nbytes, long type, int flag )` | Receive a message, selected by `type`. |
| `msgctl` | `<sys/msg.h>` | `int msgctl( int id, int cmd, struct msqid_ds *buf )` | Control a queue; `IPC_RMID` deletes it. |

Constants: `IPC_PRIVATE`, `IPC_CREAT`, `IPC_EXCL`, `IPC_NOWAIT`, `IPC_RMID`, `SIG_BLOCK`, `SIG_UNBLOCK`, `SIG_SETMASK`, `ESRCH`.

Commands: `ipcs -q` (list queues), `ipcrm -q <id>` (remove one).

Useful related: `sigpending(&set)` (which signals are pending), `sigsuspend` (atomic unblock-and-pause), `sigaction`, `SIGUSR1`/`SIGUSR2` (signals reserved for your own use), `volatile sig_atomic_t`.

## Exercises

1. [ex1_sigint_handler.c](ex1_sigint_handler.c) — catch Ctrl-C with a signal handler.
2. [ex2_message_queue.c](ex2_message_queue.c) — send one message from child to parent through a queue.

Older, longer exercises are in [old-exercises/](old-exercises/) for extra practice.
