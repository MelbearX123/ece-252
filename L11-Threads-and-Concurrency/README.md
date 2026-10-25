# Lecture 11 — Threads and Concurrency

Slides: [slides.pdf](slides.pdf)

This lecture finishes off pthreads (cancelling a thread safely) and then asks the big question: if several threads run "at the same time", what does that actually mean, and how much faster does it make things?

## 1. Thread Cancellation

**Thread cancellation** means terminating a running thread **before it has finished its work**, e.g. the user clicks Cancel on an upload. The thread being cancelled is called the **target**.

You met the two basic calls in Lecture 10:

```c
pthread_cancel( tid );    /* ask thread `tid` to stop */
pthread_testcancel();     /* "have I been cancelled? if so, stop here" */
```

`pthread_cancel` takes the target's thread id. `pthread_testcancel` takes nothing. If a cancel request is pending for the calling thread, the thread terminates right there. If you write a thread that might be cancelled, it's polite to call `pthread_testcancel` regularly, for example once per loop iteration.

### Cancellation types

| Type | What happens when a cancel arrives |
|------|-----------------------------------|
| **Deferred** (default) | The request is remembered. The thread only stops when it reaches a **cancellation point**. |
| **Asynchronous** | The thread can be stopped **immediately, at any instruction**. |

A thread chooses its own type:

```c
int pthread_setcanceltype( int type, int *oldtype );
/* type:    PTHREAD_CANCEL_DEFERRED or PTHREAD_CANCEL_ASYNCHRONOUS
   oldtype: receives the previous type, if you care (can be NULL) */
```

Asynchronous is dangerous. The thread could be killed halfway through `malloc`, or halfway through updating a shared data structure, leaving it broken. Deferred is the safe default because the thread only stops at places where it's ready to stop.

### Cancellation points

Under deferred cancellation, the thread stops at a **cancellation point**. `pthread_testcancel` is one, but it isn't the only one. POSIX requires many functions to check for cancellation implicitly, mostly ones that can block: `sleep`, `read`, `write`, `pthread_join`, `sem_wait` (Lecture 13), and many more. Even more functions are "**potential** cancellation points": they *may* check, depending on the system. So you can't assume a thread will run straight through a call like `printf` without being cancelled.

## 2. Cleanup Handlers

The danger with cancelling a thread: **it might die before cleaning up.** Memory it allocated is never freed, and things it locked stay locked forever.

```c
void *do_work( void *argument ) {
    struct job *j = malloc( sizeof( struct job ) );
    /* ... long work, containing cancellation points ... */
    free( j );                 /* if cancelled above, this NEVER runs: leak! */
    pthread_exit( NULL );
}
```

The fix is a **cleanup handler**: a function the thread registers that runs automatically **if the thread is cancelled** (or calls `pthread_exit`) while the handler is registered.

```c
void pthread_cleanup_push( void (*routine)(void *), void *argument );  /* register */
void pthread_cleanup_pop( int execute );   /* unregister; run it too if execute != 0 */
```

Handlers are kept on a stack. `push` adds one, `pop` removes the most recent one.

```c
void cleanup( void *mem ) {
    free( mem );
}

void *do_work( void *argument ) {
    struct job *j = malloc( sizeof( struct job ) );
    pthread_cleanup_push( cleanup, j );    /* if cancelled from here on: cleanup(j) runs */

    /* ... long work ... */

    free( j );
    pthread_cleanup_pop( 0 );              /* 0 = remove handler but DON'T run it */
    pthread_exit( NULL );
}
```

- In the normal case the thread frees `j` itself and pops with `0`, so the memory isn't freed twice.
- Alternatively, drop the `free( j )` line and call `pthread_cleanup_pop( 1 )`. That runs the handler, which frees `j`.
- **`push` and `pop` must be paired at the same level** of curly braces in the same function. (They're usually macros that open and close a `{ }` block, so an unpaired one won't even compile.)

## 3. Example: Summing in a Thread (with Attributes)

This example passes a thread **attributes** (`pthread_attr_t`) and doesn't use the thread's return value. Attributes let you configure a thread (e.g. joinable vs. detached). Here they're just initialized to defaults.

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum;   /* shared data: globals are shared by all threads */

void *runner( void *param ) {
    int upper = atoi( param );
    sum = 0;
    for ( int i = 1; i <= upper; i++ ) {
        sum += i;
    }
    pthread_exit( 0 );
}

int main( int argc, char **argv ) {
    pthread_t tid;
    pthread_attr_t attr;
    /* ... check argc and that argv[1] >= 0 ... */
    pthread_attr_init( &attr );                        /* default attributes */
    pthread_create( &tid, &attr, runner, argv[1] );
    pthread_join( tid, NULL );
    printf( "sum = %d\n", sum );
    pthread_attr_destroy( &attr );                     /* free the attributes object */
    pthread_exit( NULL );
}
```

Both threads share `sum`. **Is there coordination?** Yes: `main` **joins** the worker before printing, so it waits until the sum is complete. Without the join, `main` could print the sum early, while the worker is still adding.

## 4. Three Threads, One `sum`: Things Go Wrong

Now the same `runner` (without the `sum = 0` line) is started **three times**, all adding into the same global:

```c
for ( int i = 0; i < 3; ++i ) {
    pthread_create( &tid[i], NULL, runner, argv[1] );
}
for ( int j = 0; j < 3; ++j ) {
    pthread_join( tid[j], NULL );
}
printf( "sum = %d\n", sum );
```

The expected answer is 3 × (1 + 2 + … + n).

- For **small** n, the answer is right.
- For **large** n, you get **strange, inconsistent results** that differ from run to run.

Three threads are modifying `sum` "at the same time", and `sum += i` is not one indivisible step (Lecture 12 covers exactly why). It's the same bug you saw in Lecture 10's shared-counter exercise. To understand it, we first need to pin down what "at the same time" means.

## 5. What Does "At the Same Time" Mean?

### Uniprocessors and multicore

- Not long ago a typical computer had **one processor with one core**, so it could do **exactly one thing at a time**. A system with a single general-purpose processor is a **uniprocessor** system. It may still have special-purpose processors, such as a RAID controller, that don't run user processes.
- Now desktops, laptops, and phones have **multi-core** processors. A quad-core processor may be executing **four different instructions from four different threads at the same instant**.

**Terminology:** a **core** is a logical processing unit that can execute instructions. "CPU" may mean the physical chip, which contains one or more cores. The OS doesn't much care whether four cores are on four chips or one chip: either way there are four units that can run instructions.

### Who runs where

- **1 process with 1 thread:** at most **one core** is used, no matter how many exist.
- **Several processes or threads:** each can run on a different core.
- **More threads than cores:** we hope that threads block often and long enough (waiting for I/O, etc.) to leave room for the others. When that isn't enough, the OS shares cores using **time slicing**.

### Time slicing

The OS runs thread 1 for a fixed period (e.g. **20 ms**), then thread 2 for 20 ms, then thread 3, then back to thread 1, and so on. 20 ms is fast enough that, **to the user, it looks like all three run in parallel**, even on a single core. On multiple cores, time slicing still happens whenever there are more runnable threads than cores.

So on one core, "at the same time" really means **interleaved**. On several cores it can mean **truly simultaneous**. Either way, you don't control when one thread stops and another starts, which is why the `sum` example breaks.

## 6. Parallelism and Speedup

Do more threads mean the task finishes faster? **It depends on the task.**

| Task | 2× threads gives |
|------|------------------|
| Fully parallelizable | 2× speed |
| Partially parallelizable | between 1× and 2× speed |
| Not parallelizable at all | 1× speed (no gain) |

(Think of a kitchen: more cooks can chop vegetables faster, but they can't make a cake bake faster.)

### Worked example

A task takes **5 s** on one processor. It consists of:
- **0.4 s** of setup and recombination code that must run serially, and
- a loop that can be split up: **4.6 s** on one processor.

On **two** processors the loop takes 4.6 / 2 = **2.3 s**. Adding the serial 0.4 s gives **2.7 s** in total. That's a reduction of about **46%**, not the 50% you might expect, because the serial part didn't get faster.

### Amdahl's Law

Gene Amdahl generalized this. Let

- **S** = the fraction of the program that must run **serially**
- **N** = the number of processing cores

Then:

```
                1
speedup ≤ ─────────────
           S + (1 − S)/N
```

As **N → ∞**, the (1 − S)/N term goes to 0, so the speedup approaches **1/S**. **The serial part limits how much extra processors can help.**

For the 5 s task, S = 0.4 / 5 = 0.08. The run time is 0.4 + 4.6 / N:

| Processors | Run time (s) |
|-----------:|-------------:|
| 1 | 5 |
| 2 | 2.7 |
| 4 | 1.55 |
| 8 | 0.975 |
| 16 | 0.6875 |
| 32 | 0.54375 |
| 64 | 0.471875 |
| 128 | 0.4359375 |

Observations:
1. **Diminishing returns:** each doubling helps less than the one before.
2. The run time **converges on 0.4 s**, the serial part.
3. The best possible speedup is 5 / 0.4 = **12.5×**, and only with infinitely many processors (and infinite money).

### Merge sort

Merge sort is a natural fit for threads. It's divide-and-conquer: split the array into pieces, **sort the pieces in parallel** (one thread each), then **merge** the sorted pieces. The merge has to wait for the sorting threads to finish (join them first). That ordering, "sorting happens before merging", is called **serialization**, and it's the first topic of Lecture 12.

---

## New Functions Introduced

| Function | Header | Signature | Purpose |
|----------|--------|-----------|---------|
| `pthread_setcanceltype` | `<pthread.h>` | `int pthread_setcanceltype( int type, int *oldtype )` | Choose `PTHREAD_CANCEL_DEFERRED` (default) or `PTHREAD_CANCEL_ASYNCHRONOUS` for the calling thread. |
| `pthread_cleanup_push` | `<pthread.h>` | `void pthread_cleanup_push( void (*routine)(void *), void *arg )` | Register a cleanup handler that runs if the thread is cancelled or calls `pthread_exit`. |
| `pthread_cleanup_pop` | `<pthread.h>` | `void pthread_cleanup_pop( int execute )` | Remove the most recent handler; also run it if `execute` is non-zero. Must pair with `push` at the same brace level. |
| `pthread_attr_init` | `<pthread.h>` | `int pthread_attr_init( pthread_attr_t *attr )` | Initialize a thread-attributes object to the defaults. |
| `pthread_attr_destroy` | `<pthread.h>` | `int pthread_attr_destroy( pthread_attr_t *attr )` | Destroy an attributes object when you're done with it. |

From Lecture 10, used again here: `pthread_cancel`, `pthread_testcancel`, `pthread_create`, `pthread_join`, `pthread_exit`.

Useful related: `PTHREAD_CANCELED` is the value `pthread_join` gives you for a thread that was cancelled.

## Exercises

1. [ex1_cancel_cleanup.c](ex1_cancel_cleanup.c): cancel a worker thread without leaking its memory.
2. [ex2_amdahl.md](ex2_amdahl.md): speedup and Amdahl's law on paper.
