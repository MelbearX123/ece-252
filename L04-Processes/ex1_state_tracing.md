# Lecture 4 — Exercise 1: Tracing the Five-State Model

States: **New, Ready, Running, Blocked, Terminated**.

## Part A — One process

A user runs `gcc big.c`. Write the state the `gcc` process is in after each step:

1. The OS creates it and admits it.
The gcc is in ready state
2. It is scheduled and starts running.
The gcc is in the running state
3. It asks to read `big.c` from disk.
The gcc is blocked because the disk read takes a long time.
4. The disk read finishes.
The gcc is in ready
5. Its time slice runs out.
The gcc is in read state after being in running
6. It is scheduled again and finishes.
The gcc is terminated

## Part B — Legal or not?

Is each transition allowed in the five-state model? If not, say what must happen instead.

1. Blocked → Running
No, not allowed. Blocked must go back to ready before going to running
2. Running → Ready
Yes, this is called suspending
3. Ready → Blocked
No this is not allowed. The ready must run first before being blocked
