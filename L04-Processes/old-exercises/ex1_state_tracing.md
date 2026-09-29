# Lecture 4 — Exercise 1: Tracing the Five-State Model

Use the five states **New, Ready, Running, Blocked, Terminated** and the eight transitions **Create, Admit, Dispatch, Suspend, Block, Unblock, Exit, Reap**.

## Part A — Single process

A user types `gcc big.c` in a shell. Write the sequence of **(transition → resulting state)** for the `gcc` process, given:

1. The OS creates it and admits it right away.
2. It is scheduled and runs.
3. It asks to read `big.c` from disk.
4. The disk read completes.
5. Its time slice expires before it's done.
6. It's scheduled again and finishes compiling successfully.
7. The shell collects its exit code.

## Part B — Single CPU, three processes

One CPU. Processes A, B, C are all in **Ready** at t=0. The scheduler always dispatches the Ready process that has been waiting longest (A first). Fill in the table: the state of each process **at the end of** each time step.

| t | Event | A | B | C |
|---|-------|---|---|---|
| 0 | start | Ready | Ready | Ready |
| 1 | A dispatched | | | |
| 2 | A requests keyboard input | | | |
| 3 | (scheduler runs next) | | | |
| 4 | B's time slice expires | | | |
| 5 | User presses a key (A's event) | | | |
| 6 | C finishes (`return 0` from main); its parent hasn't called wait yet | | | |
| 7 | (scheduler runs next) | | | |
| 8 | C's parent calls wait | | | |
| 9 | User kills B with `kill` | | | |

Questions:
- At t=6, what is C called in UNIX terminology?
- At t=5, why doesn't A go straight to Running?
- Is there ever a moment in your table with two processes in Running? Could there be on a 4-core machine?

## Part C — Is it legal?

For each transition, say whether it's valid in the five-state model. If not, explain what would have to happen instead.

1. Blocked → Running
2. Ready → Blocked
3. New → Running
4. Running → Ready
5. Terminated → Ready
6. Blocked → Terminated
7. Ready → Terminated

## Part D — Zombies and orphans

1. Parent P forks child C. C exits immediately; P is in a long computation and doesn't call `wait` for 10 minutes. What is C during those 10 minutes? What resources is it still holding?
2. P then crashes before ever calling wait. What happens to C now? Who reaps it?
3. Instead: P exits first while C is still running. What is C called? What is C's parent pid afterwards?
4. Why might a programmer *intentionally* create an orphan?

## Part E — Termination kinds

Classify each as Normal exit, Error exit, Fatal error, or Killed by another process:

1. `grep` finishes searching and returns 0.
2. `cp` is given a source file that doesn't exist and returns 1.
3. A program dereferences a dangling pointer and gets a segmentation fault.
4. You press Ctrl-C on a running program.
5. A parent process times out waiting for its child and terminates it.
6. A web server checks at startup that port 80 is free, finds it isn't, prints a message, and returns -1.
