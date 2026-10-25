# Lecture 13 — Semaphores

Slides: [slides.pdf](slides.pdf)

Lecture 12 got mutual exclusion working with test-and-set, but waiting threads **busy-wait** and burn CPU. This lecture introduces the **semaphore**: a waiting thread is put to sleep by the OS and woken up when it's its turn.

## 1. What a Good Mutual Exclusion Solution Needs

Lecture 12's definition of mutual exclusion was informal. Any real solution should have all of these properties:

1. **Mutual exclusion applies:** at most one thread is in the critical section.
2. A thread that **halts outside** the critical section must **not interfere** with other threads. (Strict alternation failed this: if B quit, A was stuck.)
3. A thread that wants the critical section must **not be delayed indefinitely**. (No starving, and no deadlock like Attempt 3.)
4. When **nobody** is in the critical section, a thread that asks should get in **right away**.
5. **No assumptions** about what the threads will do, or how many processors there are.
6. A thread stays inside the critical section for a **finite** time only.

## 2. Back to Springfield: Waiting Without Wasting

Alice now shares the day shift with **Charlie**. Safety rules say at least one of them must watch the reactor at all times, so **they can't both be at lunch at once**. Nobody can predict when lunch starts or how long it lasts.

Before going to lunch, Alice calls Charlie:
- If he **answers**, he's at his desk, so she can go.
- If he **doesn't answer**, he's out, so she can't leave yet.

What does she do while she waits?

| Option | In computer terms | Problem |
|--------|-------------------|---------|
| Call again, nonstop, until he answers | **Busy-waiting** (Lecture 12) | Ties up the phone line and all of Alice's effort. Wastes CPU. |
| Wait 15 minutes, then call again | Sleep, then retry | Fine for a human, bad for a computer: if A sleeps 2000 ms but B finishes after 20 ms, A waits **1980 ms for nothing**. |
| **Leave a voicemail**: "call me back when you're at your desk" | **Semaphore** | Alice gets on with her work, and goes the moment Charlie calls back. |

What we want is that **call-when-finished** behaviour.

## 3. The Semaphore

A semaphore is a system of signals for communicating. Before radios, ships in visual range communicated with **flag semaphores**: someone held flags in specific positions to spell out messages.

The **computer semaphore** was invented by **Edsger Dijkstra in 1965**: a data structure for solving synchronization problems through messages. Today's version isn't exactly his original, but the core idea is unchanged after more than 50 years.

### The binary semaphore

A **binary semaphore** is a variable with two possible values, **0 and 1**. It can be initialized to either. It supports two operations:

- **`wait`:** try to enter the critical section. (Dijkstra called it **P**. Also called "decrementing".)
- **`post`:** say you're finished with it. (Dijkstra called it **V**. Many textbooks call it **`signal`**. Also called "incrementing".)

**`wait`:**
- If the value is **1**: set it to 0 and **continue** into the critical section.
- If the value is **0**: someone else is inside, so **this thread is blocked by the OS**, just as if it were waiting for a disk read. It uses no CPU while blocked.

**`post`:**
- If a thread is **blocked** waiting on this semaphore: **unblock** one of them. It gets to go in.
- Otherwise: set the value to **1**. (If it's already 1, nothing changes.)

### Analogy: the coffee shop washroom key

The washroom at a coffee shop is locked, and the key is behind the counter.
- **Nobody inside:** you get the key and go in. (`wait` succeeds immediately.)
- **Occupied:** you wait in line. (`wait` blocks.)
- **Key returned:** if someone's waiting, the employee hands the key to the **first person in line**. Otherwise the key goes back behind the counter. (`post`)

### The OS makes it work

The OS is essential. When thread A waits on a semaphore that's taken, the OS **blocks** A and knows not to schedule it. When thread B posts, the OS **unblocks** A so it can run again. No busy-waiting, and no guessing how long to sleep.

### What a semaphore does NOT let you do

- **You can't check the value.** A thread can't ask "would I block?" in advance. It can only call `wait` and either block or go straight through.
- **`post` doesn't tell you who's waiting.** When A posts, it doesn't know whether anyone was waiting, or which thread will continue.

## 4. Bad Behaviour, and the Mutex

Nothing in the semaphore's definition stops misuse. Imagine thread C, written by a programmer who is malicious and impatient: "my task is far too important to wait". C **posts before it waits**. Even if A or B is in the critical section, the post raises the semaphore, so C will likely get in too.

It isn't guaranteed (a waiting thread might be woken instead; it depends on the scheduler), but it's really bad: **one thread can let another into the critical section** and break mutual exclusion. It doesn't take a villain, either. A simple programming mistake does the same thing.

The fix is a **mutex**: a binary semaphore with **one extra rule**. **Only the thread that called `wait` may `post` it.** For a mutex the operations are usually called **lock** and **unlock**. The extra bookkeeping (remembering the owner) is a small, reasonable price.

## 5. Example: Linked List Integrity

```c
typedef struct single_node {
    void *element;
    struct single_node *next;
} single_node_t;

typedef struct single_list {
    single_node_t *head;
    single_node_t *tail;
    int size;
} single_list_t;

bool push_front( single_list_t *list, void *obj ) {
    single_node_t *tmp = malloc( sizeof( single_node_t ) );
    if ( tmp == NULL ) {
        return false;
    }
    tmp->element = obj;
    tmp->next = list->head;
    list->head = tmp;
    if ( list->size == 0 ) {
        list->tail = tmp;
    }
    ++(list->size);
    return true;
}
```

With **one thread** this is fine. With **two threads** pushing onto an empty list:

1. Thread 1 pushes **A**: it updates `head` and `tail`, and then, **just before `++size`**, a switch happens. `size` is still 0.
2. Thread 2 pushes **B**: it updates `head`. It checks `size == 0`, which is **true**, so it sets `tail = B` and increments `size` to 1.
3. Thread 1 resumes and increments `size` to 2.

The final state: `head` → B → A, but **`tail` points to B**, even though A is the last node. The list is in an **inconsistent state**. You'll see it when you remove elements:
- Remove from the front and check `head == tail` to see if this is the last element: you'll think B is the last one, and **lose A**.
- Or `head` gets updated but `tail` still points to B **after B is freed**, which leads to a segmentation fault or invalid memory access.

### The fix: a semaphore in the list

Add a semaphore to the list, initialized to **1** (meaning "available"), and wrap the shared updates in `wait` / `post`:

```c
typedef struct single_list {
    single_node_t *head;
    single_node_t *tail;
    int size;
    sem_t sem;
} single_list_t;

void single_list_init( single_list_t *list ) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    sem_init( &(list->sem), 0, 1 );
}

bool push_front( single_list_t *list, void *obj ) {
    single_node_t *tmp = malloc( sizeof( single_node_t ) );
    if ( tmp == NULL ) { return false; }
    tmp->element = obj;

    sem_wait( &(list->sem) );
    {                                   /* critical section */
        tmp->next = list->head;
        list->head = tmp;
        if ( list->size == 0 ) {
            list->tail = tmp;
        }
        ++(list->size);
    }
    sem_post( &(list->sem) );
    return true;
}
```

**Notice where the critical section starts and ends.** It only covers changes to the **shared list**. You could put `wait` / `post` around the whole function, but that's worse: it makes other threads wait unnecessarily. Including **`malloc`** is especially bad, because allocation itself can block if memory is short, and then everyone waits on it. Setting up `tmp` touches nothing shared, so it can happen outside.

## 6. General (Counting) Semaphores

A binary semaphore generalizes to a **counting** (or **general**) semaphore, which can hold any integer.

- `wait` decrements the value. **If that would make it negative, the caller blocks.**
- `post` increments it, or wakes a blocked thread.

So a semaphore initialized to **5** lets up to **5 threads** past `wait` before anyone blocks. If it was initialized to 5 and its value is currently 2, the next `wait` won't block. Think of a parking lot with 5 spots.

Some operating systems don't distinguish the two kinds at all: a binary semaphore is just a general semaphore created with value 1.

### Serialization with a semaphore (initial value 0)

This pattern isn't on the slides, but it follows directly from the definitions, and it's the semaphore version of Alice and Bob from Lecture 12. Initialize a semaphore to **0**:

- Alice calls `wait`. The value is 0, so she **blocks** until someone posts.
- Bob calls `post` **when he arrives**, which wakes Alice.

Whoever reaches the semaphore first, Alice can't continue until Bob has posted. **Bob's arrival is guaranteed to happen before Alice's departure.** If Bob arrives first, his post sets the value to 1, and Alice's later `wait` goes straight through.

## 7. Semaphore Syntax (POSIX)

```c
#include <semaphore.h>

int sem_init( sem_t *semaphore, int shared, unsigned int initial_value );
int sem_destroy( sem_t *semaphore );
int sem_wait( sem_t *semaphore );
int sem_post( sem_t *semaphore );
```

- `shared`: **0** = shared between the **threads of this process** (what you'll use). Non-zero = shared between processes, in which case the `sem_t` must live in shared memory (Lecture 9).
- `initial_value`: **1** for mutual exclusion, **0** for "wait until signalled", **N** to allow N threads at once.
- Call `sem_destroy` when no thread will use the semaphore any more.
- All four return 0 on success and -1 on error (setting `errno`).
- Compile with `-pthread`.

---

## New Functions Introduced

| Function | Header | Signature | Purpose |
|----------|--------|-----------|---------|
| `sem_init` | `<semaphore.h>` | `int sem_init( sem_t *sem, int shared, unsigned int value )` | Initialize a semaphore with a starting value. `shared = 0` means between threads. |
| `sem_destroy` | `<semaphore.h>` | `int sem_destroy( sem_t *sem )` | Destroy a semaphore that's no longer in use. |
| `sem_wait` | `<semaphore.h>` | `int sem_wait( sem_t *sem )` | Decrement; **block** if the value would go below 0. (P / "wait") |
| `sem_post` | `<semaphore.h>` | `int sem_post( sem_t *sem )` | Increment, or wake one blocked thread. (V / "post" / "signal") |

Type: `sem_t`.

## Exercises

1. [ex1_list_semaphore.c](ex1_list_semaphore.c): watch the lecture's linked list break under threads, then fix it with a semaphore.
2. [ex2_alice_bob.c](ex2_alice_bob.c): make Alice wait for Bob using a semaphore.
