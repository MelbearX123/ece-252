# Lecture 10 — Threads

## 1. What a Thread Is

Recall a process = executable program + its data + execution context (open files, allocated resources…). **A process has at least one thread, and can have many.**

A **thread** (short for *thread of execution*) is a sequence of instructions that can be **scheduled to run on a CPU**, with its own state (where it is in that sequence) and its own local variables. Every program you've written so far had one thread: statements execute one at a time in sequence.

A **multithreaded** program uses more than one thread at least some of the time. It starts with the **main thread** (running `main`), which can create more. Threads can be created and destroyed dynamically — e.g. one created to write changes to a database in the background that terminates when done — or be long-lived.

### What each thread has vs. what they share

| **Per thread** (private) | **Per process** (shared by all its threads) |
|--------------------------|---------------------------------------------|
| Execution state (running, ready, blocked…) | Code (the program) |
| Saved context (registers, PC) when not running | Global variables and **heap** |
| **Its own stack** | Open files, sockets, other resources |
| Local variables (they live on its stack) | The address space itself |

"All the threads of a process share the state and resources of the process." If one thread opens a file, the others can use it too.

### Why threads — the responsive UI example

A file-transfer program with the UI and upload in one thread: once the upload starts, the UI freezes ("Not Responding") — the user can't even click Cancel. Fix: do the upload in a separate **process** (`fork`) *or* a separate **thread**. Either way the UI stays responsive.

## 2. Why Threads Rather Than Processes?

Mainly **performance**:

1. **Creating** a thread is much faster than creating a process — on the order of **10×**.
2. **Terminating / cleaning up** a thread is faster too.
3. **Switching** between threads of the same process is faster (less state to save/restore). E.g. in Solaris, process switches were ~5× slower than thread switches.
4. **Communication is direct**: threads share memory, so they can talk without any IPC mechanism (pipes, queues, shared memory setup…). Inter-task communication is expensive, so this is a big deal.
5. **Responsiveness**: part of the program can be blocked while the rest keeps working.

Four common uses of threads in general-purpose systems:

1. **Foreground and background work** — keep the program responsive.
2. **Asynchronous processing** — e.g. periodically auto-saving a document to disk in the background (how Word "recovers" documents).
3. **Speed of execution** — if one thread blocks (e.g. on disk), another runs; and multiple cores can run threads truly in parallel.
4. **Modular structure** — give each thread a specific "job" that it steps in to do when appropriate.

### Drawbacks

- **No protection between threads** of a process — one thread can easily corrupt memory another is using. This requires **coordination** (synchronization), the next topic of the course.
- **Fault isolation is lost**: if any thread hits a fatal error (division by zero, segfault), the OS may terminate **the whole process** — all threads. With separate processes, the others would survive.

## 3. Thread States

Each thread has its own state, following the **five-state model** (New, Ready, Running, Blocked, Terminated) with the same transitions as processes. (The seven-state swapping model isn't needed — "if a process is blocked, we don't really care why.")

- A thread can go to Terminated from any state.
- **When a process terminates, all its threads terminate**, whatever state they're in.
- Terminating a thread from outside (e.g. the user cancels the upload) is **thread cancellation**.

## 4. POSIX Threads (pthreads)

**pthreads** = the POSIX standard (IEEE 1003.1c) specifying thread behaviour on UNIX-like systems (Linux, macOS, Solaris…), so code ports between them. ~100 functions; the key ones parallel the process calls:

| pthread call | Process analogue | Purpose |
|--------------|------------------|---------|
| `pthread_create` | `fork` | Create a new thread. |
| `pthread_exit` | `exit` | Terminate the calling thread, returning a value. |
| `pthread_join` | `wait` | Wait for a **specific** thread to exit and get its return value. **Error to join a thread twice.** |
| `pthread_detach` | — | Make a thread non-joinable (its resources are reclaimed automatically when it ends). |
| `pthread_yield` | — | Voluntarily give up the CPU so another thread can run (threads of one program cooperate rather than compete). Now deprecated in favour of `sched_yield()`. |
| `pthread_attr_init` | — | Create/initialize a thread attributes object (priority, detached state, …). |
| `pthread_attr_destroy` | — | Free an attributes object (doesn't affect threads created with it). |
| `pthread_cancel` | `kill` | Request cancellation of a thread — asynchronous or deferred depending on the thread's settings. |
| `pthread_testcancel` | — | A cancellation point: if this thread has a pending cancel, terminate here. |

Header `<pthread.h>`; compile with **`-pthread`**.

### Creating a thread

```c
int pthread_create( pthread_t *thread,
                    const pthread_attr_t *attr,
                    void *(*start_routine)( void * ),
                    void *arg );
```

- `thread` — out-parameter; filled with the new thread's ID.
- `attr` — attributes, or `NULL` for defaults (fine for nearly everything in this course).
- `start_routine` — the function the thread runs. **Must** take one `void *` and return a `void *`:
  ```c
  void *do_something( void *start_params );
  ```
- `arg` — the single argument passed to `start_routine`.
- Returns 0 on success, an error number otherwise (it does **not** set `errno`).

After creation there are two threads; **no guarantee which runs next** — either, both at once on different cores, or some other process entirely.

### Passing multiple arguments (and returning multiple values)

Only one `void *` goes in. Two options:

- Pass a pointer to an **array**.
- (Better) define a **struct** for the parameters (and another for the results), and pass a pointer to it.

Inside the thread, cast back to the real type:

```c
void *function( void *void_arg ) {
    parameters_t *arguments = (parameters_t *) void_arg;
    /* continue after this */
}
```

The caller must know what type the function expects — the `void *` removes the compiler's type "hints."

⚠️ Lecture 1 warning, now critical: whatever `arg` points to must **still be valid when the thread reads it**. Passing `&i` of a loop variable, or a local of a function that returns, is a classic bug. Use per-thread heap structs or an array indexed per thread.

### Attributes: joinable vs. detached

- By default threads are **joinable** (another thread can `pthread_join` them).
- It's a logical error to join the same thread more than once, or to join a **detached** thread.
- Create detached via attributes, or call `pthread_detach` on a joinable thread.

### How a thread ends

1. Calls **`pthread_exit(value)`**.
2. **Returns** from its start routine — same effect as `pthread_exit(return_value)`. (`return NULL;` if there's nothing to return.)
3. Is **cancelled** with `pthread_cancel`. With *deferred* cancellation the thread is responsible for cleaning up before it stops (at cancellation points like `pthread_testcancel`).
4. **The whole process ends** — e.g. `exit()` anywhere, or **`main` returns**. If `main` returns without waiting, all other unjoined threads are **suddenly terminated**. If `main` calls **`pthread_exit`** instead, the process stays alive until the other threads finish.

To get a return value out, the thread **must exit** (not linger).

### Collecting a return value: `pthread_join`

```c
int pthread_join( pthread_t thread, void **retval );
```

`retval` is a **pointer to a `void *`**: you pass the address of your own `void *` variable, and join fills it with whatever the thread returned. Pass `NULL` if you don't need it.

### Full example

```c
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *run( void *argument ) {
    char *a = (char *) argument;
    printf( "Provided argument is %s!\n", a );
    int *return_val = malloc( sizeof( int ) );   /* heap: outlives the thread */
    *return_val = 99;
    pthread_exit( return_val );
}

int main( int argc, char **argv ) {
    if ( argc != 2 ) {
        printf( "Invalid args.\n" );
        return -1;
    }
    pthread_t t;
    void *vr;

    pthread_create( &t, NULL, run, argv[1] );
    pthread_join( t, &vr );                      /* vr now points to the int */
    int *r = (int *) vr;
    printf( "The other thread returned %d.\n", *r );
    free( vr );                                  /* joiner frees what the thread allocated */
    pthread_exit( 0 );
}
```

Why `malloc` the return value? The thread's **stack is gone** after it exits, so returning the address of a local would be a dangling pointer. Ownership passes to the joiner, who must `free` it.

---

## New Functions Introduced

| Function | Header | Signature | Purpose |
|----------|--------|-----------|---------|
| `pthread_create` | `<pthread.h>` | `int pthread_create( pthread_t *t, const pthread_attr_t *attr, void *(*fn)(void *), void *arg )` | Start a new thread running `fn(arg)`. |
| `pthread_exit` | `<pthread.h>` | `void pthread_exit( void *retval )` | Terminate the calling thread with a return value. In `main`: wait for other threads before the process ends. |
| `pthread_join` | `<pthread.h>` | `int pthread_join( pthread_t t, void **retval )` | Wait for thread `t` and collect its return value. |
| `pthread_detach` | `<pthread.h>` | `int pthread_detach( pthread_t t )` | Make a thread non-joinable; auto-cleanup when it ends. |
| `pthread_yield` | `<pthread.h>` | `int pthread_yield( void )` | Give up the CPU (deprecated; use `sched_yield()` from `<sched.h>`). |
| `pthread_attr_init` | `<pthread.h>` | `int pthread_attr_init( pthread_attr_t *attr )` | Initialize a thread attributes object. |
| `pthread_attr_destroy` | `<pthread.h>` | `int pthread_attr_destroy( pthread_attr_t *attr )` | Destroy an attributes object. |
| `pthread_cancel` | `<pthread.h>` | `int pthread_cancel( pthread_t t )` | Request cancellation of a thread. |
| `pthread_testcancel` | `<pthread.h>` | `void pthread_testcancel( void )` | Act on a pending cancellation request (terminates the caller if cancelled). |

Types: `pthread_t`, `pthread_attr_t`.

Useful related (used in exercises): `pthread_self()` (my own thread id), `pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)`, `PTHREAD_CANCELED` (the value `join` returns for a cancelled thread), `pthread_setcanceltype`, `clock_gettime(CLOCK_MONOTONIC, …)` for timing, `strerror(rc)` for pthread error codes.

## Exercises

1. [ex1_hello_threads.c](ex1_hello_threads.c) — create and join threads, passing each an argument.
2. [ex2_shared_counter_race.c](ex2_shared_counter_race.c) — see a race condition on a shared counter.

Older, longer exercises are in [old-exercises/](old-exercises/) for extra practice.
