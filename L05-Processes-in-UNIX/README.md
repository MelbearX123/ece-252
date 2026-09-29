# Lecture 5 — Processes in UNIX

## 1. The UNIX Process Tree

Every UNIX process has a parent, all the way back to **`init`** (or `launchd` on macOS, `systemd` on many Linux distros) at the root. Each process has a unique **pid** (process ID). `init` always has **pid 1** — don't try to kill it.

```
          init (1)
        /    |     \
   sshd    screen   login
  (4881)   (7574)   (1490)
     |                 |
   bash              bash
  (4310)            (5911)
   /    \              |
 gcc   pdflatex       vim
```

- Logging in spawns a `login` process, which spawns your **shell** (usually `bash`).
- When you type a command (`ls`, `top`), the shell creates a new process and **waits** for it to finish before showing the prompt again.
- `ps` lists processes; `top` shows a live table of processes.

### Running things in the background

**`&`** — start the command without waiting for it:

```bash
gcc fork.c &
[1] 34429          # job number and pid of the child
...
[1]+ Done  gcc fork.c
```

- Output still goes to the same terminal; redirect it if you don't want it mixed in: `cat fork.c > logfile.txt &`.
- Typical use: `sudo service xyz start &` then immediately `tail -f /var/log/xyz/console.log` to watch it start.

**`screen`** — for *interactive* programs. `screen vi fork.c` runs `vi` inside a screen session that you can **detach** from (back to the shell) and **reattach** to later — it even survives logging out. See `man screen`. (`tmux` is a modern alternative.)

## 2. `fork`, `exec`, `wait`, `exit`

The UNIX workflow:

1. Parent calls **`fork()`** to create a child.
2. Optionally, one of them calls **`exec`** to replace itself with a different program.
3. If the parent cares about the result, it calls **`wait()`** and blocks until the child finishes.
4. The child finishes with **`exit(code)`** (or by returning from `main`). The parent receives that code from `wait`.

### `fork()`

Creates a new process that is a **copy of the caller** — same code, a copy of the same memory, same open files. **Both processes continue from the line after `fork`.** The only way to tell them apart is the return value:

| `fork()` returns | Meaning |
|------------------|---------|
| **< 0** | Fork failed; no child was created (you're still the only process). |
| **0** | You are the **child**. |
| **> 0** | You are the **parent**; the value is the **child's pid**. |

After the fork, parent and child have **separate** copies of memory: changing a variable in one does not affect the other.

### `exec` family

`exec` **replaces the current process's memory image** with a new program loaded from a binary file and starts it at its `main`. The pid stays the same. If `exec` succeeds, **it never returns** — the code after it is gone. If it returns, it failed.

```c
execlp( "/bin/ls", "ls", NULL );   /* file, argv[0], argv[1], ..., NULL */
```

- `l` = arguments passed as a **list**; ends with `NULL`.
- `p` = search the **PATH** for the program if no `/` in the name.
- The second argument is what the new program sees as `argv[0]` (by convention, its name).
- There's no rule a child must `exec`; it can stay a clone of the parent and run different code in the same source file.

### `wait()`

```c
pid_t wait( int *status );
```

Blocks the parent until **a** child terminates, then returns that child's pid and fills `status` with a **status word** — not simply the return code. Decode it with macros from `<sys/wait.h>`:

- `WIFEXITED(status)` — true if the child exited normally.
- `WEXITSTATUS(status)` — the child's exit code (only the **low 8 bits**, 0–255).
- (Related: `WIFSIGNALED(status)` / `WTERMSIG(status)` if it was killed by a signal.)

Calling `wait` is what **reaps** the child — without it, a finished child remains a **zombie** (Lecture 4).

### `exit()`

```c
void exit( int status );
```

Terminates the calling process with the given status. `return x;` at the end of `main` is equivalent to `exit(x)`.

### The canonical example

```c
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main( int argc, char **argv ) {
    pid_t pid;
    int childStatus;

    pid = fork();

    if ( pid < 0 ) {                 /* error */
        fprintf( stderr, "Fork Failed" );
        return 1;
    } else if ( pid == 0 ) {         /* child */
        execlp( "/bin/ls", "ls", NULL );
    } else {                         /* parent */
        wait( &childStatus );
        printf( "Child Complete with status word: %i\n", childStatus );
    }
    return 0;
}
```

Timeline: after `fork`, two processes reach the `if`. The child replaces itself with `ls`, which lists the directory and exits. The parent blocks in `wait` until then, then prints "Child Complete…".

### Design problem: same source, two jobs

Split a task into `execute_A()` (parent) and `execute_B()` (child); either returns non-zero on error. Report `Error N Occurred.` for each error, or `Completed.` if both succeed.

```c
int main( int argc, char **argv ) {
    pid_t pid;
    int child_result;
    int parent_result;

    pid = fork();
    if ( pid < 0 ) {                 /* fork failed */
        return -1;
    } else if ( pid == 0 ) {         /* child */
        return execute_B();          /* becomes the child's exit code */
    } else {                         /* parent */
        parent_result = execute_A();
        wait( &child_result );
    }

    if ( child_result == 0 && parent_result == 0 ) {
        printf( "Completed.\n" );
        return 0;
    }
    if ( child_result != 0 ) {
        printf( "Error %d Occurred.\n", WEXITSTATUS( child_result ) );
    }
    if ( parent_result != 0 ) {
        printf( "Error %d Occurred.\n", parent_result );
    }
    return -1;
}
```

Key points: the child's return value is delivered through `wait`'s status word, so you need `WEXITSTATUS` to extract it; checking `child_result != 0` works as a quick "did anything go wrong" test because a normal exit with code 0 produces a status word of 0. And both parent and child execute concurrently: `execute_A` and `execute_B` run at the same time.

## 3. The Fork Bomb (don't)

A **denial-of-service** attack: a program that forks repeatedly, where every copy also forks. After *n* rounds there are **2ⁿ** processes, exhausting the system's process table / CPU so it crashes or becomes unusable.

Defences: limit the **total number of processes** per user, and the **rate** at which a user may create them (see `ulimit -u`).

⚠️ Never run one on shared machines (ecelinux) — it's misuse of resources and can get you banned.

## 4. Signals

A **signal** is a notification to a process that an event occurred — essentially a (software) **interrupt with an integer ID**.

- **Synchronous**: caused by the program's own execution (divide by zero → `SIGFPE`, bad memory access → `SIGSEGV`).
- **Asynchronous**: comes from outside the process (user presses Ctrl-C, another process sends one).

Every signal must be **handled** — even if the handling is "ignore it." By default the kernel uses the **default handler**, whose action depends on the signal.

| Signal | Meaning | Value | Default action |
|--------|---------|-------|----------------|
| `SIGHUP` | Hangup detected (terminal closed) | 1 | Terminate |
| `SIGINT` | Keyboard interrupt (**Ctrl-C**) | 2 | Terminate |
| `SIGQUIT` | Quit from keyboard (Ctrl-\\) | 3 | Terminate + core dump |
| `SIGILL` | Illegal instruction | 4 | Terminate + core dump |
| `SIGKILL` | Kill | 9 | Terminate — **cannot be caught, blocked, or ignored** |
| `SIGSEGV` | Segmentation fault (invalid memory reference) | 11 | Terminate + core dump |
| `SIGTERM` | Termination request | 15 | Terminate |
| `SIGCHLD` | Child stopped or terminated | 20/17/18 | Ignore |
| `SIGCONT` | Continue if stopped | 19/18/25 | Continue |
| `SIGSTOP` | Stop (pause) process | 18/20/24 | Stop — **cannot be caught, blocked, or ignored** |

(Several values differ by architecture — always use the **names**, not the numbers.)

A process can instead tell the OS it will **handle a signal itself** (e.g. clean up on Ctrl-C before exiting) — covered in Lecture 6.

### Sending signals from the command line

- `kill 24601` sends **SIGTERM** (the polite request — the process can clean up).
- `kill -9 24601` sends **SIGKILL** (forced; no cleanup possible).
- Etiquette: try `SIGTERM`, `SIGINT`, or `SIGHUP` first; reach for `-9` only if the process is truly stuck.

Signals are a (very limited) form of **inter-process communication** — which leads into Lecture 6.

---

## New Functions Introduced

| Function / Macro | Header | Signature | Purpose |
|------------------|--------|-----------|---------|
| `fork` | `<unistd.h>` | `pid_t fork( void )` | Create a child process (copy of caller). Returns 0 in child, child pid in parent, <0 on error. |
| `execlp` | `<unistd.h>` | `int execlp( const char *file, const char *arg0, ..., (char *) NULL )` | Replace current process image with a new program (PATH search, list args). Returns only on error. |
| `wait` | `<sys/wait.h>` | `pid_t wait( int *status )` | Block until a child terminates; reap it and get its status word. |
| `exit` | `<stdlib.h>` | `void exit( int status )` | Terminate the calling process with a status code. |
| `WEXITSTATUS` | `<sys/wait.h>` | `int WEXITSTATUS( int status )` | Extract the exit code (0–255) from a status word. |
| `pid_t` | `<sys/types.h>` | type | Integer type for process IDs. |
| `fprintf(stderr, …)` | `<stdio.h>` | | Print to the standard error stream. |

Commands: `ps`, `top`, `&`, `screen`, `kill`, `kill -9`.

Useful related (not formally in the lecture, used in the exercises): `getpid()` / `getppid()` (own pid / parent's pid, `<unistd.h>`), `waitpid(pid, &status, 0)` (wait for a *specific* child — appears in Lecture 9), `WIFEXITED`, `WIFSIGNALED`, `WTERMSIG`, `sleep(seconds)`, other exec variants (`execvp`, `execl`, `execv`).

## Exercises

1. [ex1_fork_basics.c](ex1_fork_basics.c) — fork return values and separate memory.
2. [ex2_run_command.c](ex2_run_command.c) — fork + exec + wait.

Older, longer exercises are in [old-exercises/](old-exercises/) for extra practice.
