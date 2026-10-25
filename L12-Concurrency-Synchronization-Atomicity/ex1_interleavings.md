# Lecture 12 — Exercise 1: Tracing Interleavings

An **interleaving** is one possible order the steps of two threads could run in. For each question, write your interleaving as a list of steps, e.g. `A1, B1, A2`.

## Part A — Shared variable

| Thread A | Thread B |
|----------|----------|
| A1. `x = 1` | B1. `x = x + 10` |
| A2. `print x` | |

Assume `x` starts at 0, and that each line runs as one step.

1. List every possible interleaving. For each one, write what's printed and the final value of `x`.
2. Is any of these outcomes "wrong"? What would you need if exactly one of them were the correct one?

## Part B — `count++` really is three steps

`count` starts at 0. Two threads each run `count++` once. Split each increment into its three steps: read, add, write.

1. Write an interleaving where the final value is **2**.
2. Write an interleaving where the final value is **1**. Which increment was lost?

## Part C — Breaking the flag attempts

1. **Attempt 2 (busy flag):** write an interleaving where **both** threads end up inside the critical section.
2. **Attempt 3 (one flag per thread):** write an interleaving where **both** threads get stuck waiting forever.
3. Why does test-and-set fix Attempt 2? Answer in one or two sentences.
