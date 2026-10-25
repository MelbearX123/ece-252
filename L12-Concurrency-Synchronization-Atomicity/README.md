# Lecture 12 — Concurrency: Synchronization and Atomicity

Slides: [slides.pdf](slides.pdf)

Lectures 10 and 11 showed threads giving wrong answers when they share data. This lecture explains **why** that happens, and works step by step toward a way to stop it.

## 1. Synchronization

If a computer only ever did exactly one thing at a time, there'd be no concurrency and no coordination problems. But:

- A system with multiple processes or threads has **concurrency**.
- If it's multicore, it can also have **parallelism** (truly simultaneous execution).

Either one can cause problems. The key fact: **you, the programmer, don't decide when a thread runs or when a switch happens.** The OS does, and it doesn't care whether that's a convenient moment for your program.

In everyday English, "synchronization" means making things happen at exactly the same time. In computing it's more general: **any relationship between events**, such as *before*, *during*, or *after*. There are two main kinds.

## 2. Serialization: "A before B"

**Serialization** means guaranteeing an **order** of events: we're certain event A happens before event B.

- **Example:** in merge sort, the merge thread must wait for the sorting threads to finish. Sorting (A) must happen before merging (B).
- In code, the simplest form is one thread **waiting** for another, as with `pthread_join` or `wait`.

### Serialization through messages: Alice and Bob

Alice and Bob work at the Springfield Nuclear Power Plant, in separate offices where they can't see each other. Alice works days and Bob works nights. Safety rules say **Alice can't go home until Bob has arrived**. So Bob's arrival must happen before Alice's departure.

Solution: **Alice doesn't leave until Bob calls her, and Bob only calls once he's in the office.** This is the "text me when you get here" pattern from real life. One side waits for a message, and the other sends it when the event has happened.

## 3. Mutual Exclusion: "not C and D at the same time"

**Mutual exclusion** means two events **must not happen at the same time**, in either order.

- **Example:** a shared memory area between two processes (Lecture 9). If P1 writes to it (event C) while P2 is reading it (event D), P2 could read half-old, half-new data. With mutual exclusion, C and D never overlap.

## 4. Sequential vs. Concurrent Events

- Two events are **sequential** if we **know their order**. We know Bob arrives before Alice leaves.
- Two events are **concurrent** if we **can't tell from the program which happens first**. Alice and Bob each eat lunch at some point, and we have no idea who ate first.

This is the formal, strict meaning. "Concurrent" doesn't mean "at exactly the same time". It means **no order is known or guaranteed**. Alice might eat at 12:00 and Bob at 13:00, or both at 12:00, or Bob first. We can't know.

### Nondeterminism

The order of concurrent events can **change between two runs** of the same program. That's **nondeterminism**. If one thread prints "1" and another prints "2", with no coordination, you might see `12` or `21`.

This is what makes concurrency bugs so painful: if `12` is correct but `21` happens only rarely, the bug is hard to reproduce and hard to find.

### Heisenbugs

A bug like that is called a **Heisenbug**, after Werner Heisenberg's uncertainty principle (not *Breaking Bad*). The harder you look for it, the less likely it is to happen. Adding print statements or running in a debugger changes the timing enough that the bad interleaving goes away.

## 5. Shared Data and Atomic Operations

All the threads of a process share the same data, so any shared variable is a potential problem. A simple example with a shared `x`:

| Thread A | Thread B |
|----------|----------|
| A1. `x = 5` | B1. `x = 7` |
| A2. `print x` | |

With no coordination, the possible outcomes are:

- `5` is printed and the final value is 5 (order: B1, A1, A2)
- `7` is printed and the final value is 7 (order: A1, B1, A2)
- `5` is printed and the final value is 7 (order: A1, A2, B1)

But **printing 7 with a final value of 5 is impossible**. For A2 to print 7, B1 must have run after A1. Then nothing runs after B1 that sets x back to 5.

### You don't even need threads: interrupts

An app counts occurrences of some event in a variable `count`, using `count++`. The user also has a **reset button**, which triggers an interrupt that sets `count = 0`.

`count++` looks like one statement, but the CPU does it in **three steps**:

1. **Read** `count` (reads 4)
2. **Add** 1 (now 5)
3. **Write** it back (writes 5)

Now suppose the reset interrupt arrives at the worst moment:

1. Read `count` (4)
2. Add 1 (5, held in a register)
3. **INTERRUPT:** the handler runs and writes 0 to `count`
4. **END INTERRUPT:** control returns where it left off
5. Write 5 back to `count`

`count` is now 5, but the user pressed reset, so it should be 0 or 1. **The reset was lost.** If the interrupt had come before step 1, the result would be 1. If it had come after step 3, it would be 0. Either would be fine.

### Atomic operations

The problem is that `count++` is really three operations and **can be interrupted between any of them**. An operation that **can't be interrupted partway** is called **atomic**, meaning indivisible, from the Greek *atomos*, "uncuttable".

Some atomic operations are available on any system, but **you can't assume an operation is atomic**. In fact most aren't.

## 6. Critical Sections

Could serialization fix the `count` problem? No. Serialization needs a correct order ("first this, then that"), but here **both orders are valid**: the reset can come before or after an event. What we need is that the two operations **don't overlap**. That's **mutual exclusion** (often shortened to **mutex**). The reset has to wait until `count++` has completely finished, or the other way around.

A **critical section** is a piece of code that **at most one thread** may be running at a time. Usually that's code that touches shared data.

- The goal of mutual exclusion: **at most one thread is in the critical section at any moment.** More than one means something has gone badly wrong.
- But the critical section has to do something useful, so you can't "solve" the problem by never letting anyone in.
- **Finding** critical sections is the programmer's job. Tools can help, but the best tool is reading the code carefully.
- Keep critical sections **as short as possible**, while still covering *every* access to the shared data. Only one thread can run a critical section at a time, so it's serial code, and it **increases S** in Amdahl's law (Lecture 11).

## 7. Attempts at Mutual Exclusion with Flags

Each of these attempts uses an ordinary variable to guard the critical section. Each one fails, and seeing *how* it fails is the point.

### Attempt 1: take turns (strict alternation)

| Thread A | Thread B |
|----------|----------|
| `while ( turn != 0 ) { /* wait */ }` | `while ( turn != 1 ) { /* wait */ }` |
| *critical section* | *critical section* |
| `turn = 1;` | `turn = 0;` |

This forces **strict alternation**: A, B, A, B, …

- What if A needs to run more often than B? A has to wait for B's turn every time.
- **If B terminates, A is stuck forever**, because `turn` will always say it's B's turn.

❌ Not satisfactory.

### Attempt 2: a "busy" flag

| Thread A | Thread B |
|----------|----------|
| `while ( busy == true ) { /* wait */ }` | `while ( busy == true ) { /* wait */ }` |
| `busy = true;` | `busy = true;` |
| *critical section* | *critical section* |
| `busy = false;` | `busy = false;` |

Checking `busy` and setting it are **two separate steps**: a read, then a write. If a switch happens between them, both threads see `busy == false`, both set it to true, and **both enter the critical section at once**.

❌ Not satisfactory.

### Attempt 3: one flag per thread

| Thread A | Thread B |
|----------|----------|
| `flag[0] = true;` | `flag[1] = true;` |
| `while ( flag[1] ) { /* wait */ }` | `while ( flag[0] ) { /* wait */ }` |
| *critical section* | *critical section* |
| `flag[0] = false;` | `flag[1] = false;` |

Each thread announces "I want in" first, then waits until the other doesn't want in. But if A sets `flag[0]`, then a switch happens and B sets `flag[1]`, **each waits for the other forever**. Neither can move.

That's slightly better than two threads in the critical section, but two threads permanently stuck isn't acceptable either.

❌ Not satisfactory.

## 8. Disabling Interrupts

Every attempt above was beaten by a **badly timed switch**, and switches are triggered by interrupts. So what if we just **turn interrupts off** during the critical section? It's a crude solution:

- While interrupts are off, the system **can't respond** to user input or other events (a fire alarm, an incoming-missile detector…).
- If the program hits an error in the critical section and dies, the system is effectively **stuck**: nothing else can run.
- On a **multiprocessor** it **doesn't even work**. Disabling interrupts on one core doesn't stop a thread on another core from entering the critical section.

But getting the **hardware** involved is the right idea.

## 9. Test-and-Set

Hardware designers saw this problem coming and provided the **Test-and-Set** instruction. It's a single machine instruction that **can't be interrupted**: an **atomic read and write** together. Its meaning in C:

```c
bool test_and_set( int *i ) {
    if ( *i == 0 ) {
        *i = 1;
        return true;     /* it was free, and now it's mine */
    } else {
        return false;    /* someone else already has it */
    }
}
```

(Written in C like this, it is **not** atomic. The point is that the hardware does all of it in one step.)

Using it, both threads run the same code:

```c
while ( !test_and_set( &busy ) ) {
    /* wait for my turn */
}
/* critical section */
busy = 0;
```

This fixes Attempt 2: the check and the set happen in one indivisible step, so two threads can never both see "free". **Mutual exclusion achieved**, and no thread gets stuck thinking another is inside.

### Busy-waiting

There's still a cost. The `while` loop keeps checking, over and over. That's **busy-waiting**: the waiting thread burns CPU time doing nothing useful, time another thread could have used.

- It's less wasteful to wait a little between checks (sleep, then retry).
- Better still: Alice and Bob used a **message** for serialization ("call me when you arrive"). Messages can provide mutual exclusion too, letting a waiting thread *sleep until it's told to go*. That's the **semaphore**, in Lecture 13.

---

## New Functions Introduced

None from the C library. This lecture is about concepts. The one new "operation" is the hardware **test-and-set** instruction.

If you want a real atomic test-and-set in C (used in [ex2](ex2_test_and_set.c)), C11 provides one in `<stdatomic.h>`:

| Function | Header | Signature | Purpose |
|----------|--------|-----------|---------|
| `atomic_flag_test_and_set` | `<stdatomic.h>` | `bool atomic_flag_test_and_set( volatile atomic_flag *f )` | Atomically set the flag and return its **previous** value. |
| `atomic_flag_clear` | `<stdatomic.h>` | `void atomic_flag_clear( volatile atomic_flag *f )` | Atomically reset the flag to clear. |

⚠️ The return value is the **opposite** of the lecture's version. C11 returns the *old* value, so `false` means "it was free and now it's yours". The exercise wraps it in a helper with the lecture's meaning, so you don't have to worry about this.

## Exercises

1. [ex1_interleavings.md](ex1_interleavings.md): trace interleavings by hand and break the flag attempts.
2. [ex2_test_and_set.c](ex2_test_and_set.c): fix Lecture 10's shared-counter race with test-and-set.
