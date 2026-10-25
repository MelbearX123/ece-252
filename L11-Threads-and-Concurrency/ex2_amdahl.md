# Lecture 11 — Exercise 2: Amdahl's Law

Amdahl's law: with a serial fraction **S** and **N** cores,

```
                1
speedup ≤ ─────────────
           S + (1 − S)/N
```

Show your work for each question.

## Part A — A 10 second task

A program takes **10 s** on one core. **2 s** of that is setup that can't be parallelized; the other **8 s** is a loop that splits perfectly across cores.

1. What is S?
2. How long does it take on 2 cores? On 4 cores?
3. What is the best possible speedup, no matter how many cores you buy? What run time does that correspond to?

## Part B — Is it worth it?

Your boss offers to upgrade your machine from 8 cores to 16 for the program in Part A.

1. How much faster (in seconds) does the program get?
2. Compare that with the gain from going from 1 core to 2. What does this show?

## Part C — Connecting to the lecture

1. In Lecture 11's three-thread `sum` example, the threads interfered with each other. In Lecture 12 you'll fix that by letting only one thread at a time touch `sum`. What does that do to S, and therefore to the speedup?
2. Why does a single-threaded program get no benefit from a 16-core machine?
