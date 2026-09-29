# Lecture 4 — Exercise 1: Tracing the Five-State Model

States: **New, Ready, Running, Blocked, Terminated**.

## Part A — One process

A user runs `gcc big.c`. Write the state the `gcc` process is in after each step:

1. The OS creates it and admits it.
2. It is scheduled and starts running.
3. It asks to read `big.c` from disk.
4. The disk read finishes.
5. Its time slice runs out.
6. It is scheduled again and finishes.

## Part B — Legal or not?

Is each transition allowed in the five-state model? If not, say what must happen instead.

1. Blocked → Running
2. Running → Ready
3. Ready → Blocked
