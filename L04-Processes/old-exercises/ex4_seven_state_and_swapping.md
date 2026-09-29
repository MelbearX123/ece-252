# Lecture 4 — Exercise 4: Swapping and the Seven-State Model

States: **New, Ready, Running, Blocked, Ready/Swapped, Blocked/Swapped, Terminated**.

## Part A — Draw it

From memory, draw the seven-state diagram with every labelled transition (Create, Admit, Dispatch, Suspend, Block, Unblock ×2, Swap In, Swap Out, Exit, Reap). Then check against the notes. Which transitions did you forget?

## Part B — Why seven and not six?

Explain in your own words the two problem scenarios that show a single "Swapped" state is insufficient:

1. ______________________________________________
2. ______________________________________________

## Part C — Choosing a victim

Memory is full and the OS must swap one process out. Rank these candidates from best to worst choice and justify:

- P1: Blocked, waiting for a network packet expected in ~5 seconds.
- P2: Ready, high priority, will probably be dispatched next.
- P3: Blocked, waiting for the user to type something (user went to lunch).
- P4: Ready, low priority.
- P5: Running.

## Part D — Trace it

A process P goes through these events. Give the state after each.

1. P is created. (Assume admission puts new processes on disk by default.)
2. Swapped in.
3. Dispatched.
4. Requests a disk read.
5. Memory pressure: P is swapped out.
6. The disk read completes.
7. Memory frees up; P is swapped in.
8. Dispatched.
9. Finishes.
10. Parent calls wait.

At step 6, why doesn't the OS immediately swap P back in to check whether it's ready?

## Part E — Cost

Swapping a 2 GB process out to an SSD writing at 500 MB/s and back in later:

a. How long does the round trip take (ignore everything but raw transfer)?
b. How many 3 GHz CPU cycles is that?
c. Why does the OS prefer to swap a *blocked* process rather than a *ready* one?
d. Look up "thrashing". How does it relate to swapping ready processes too aggressively?

## Part F — Short answers

1. Why are PCBs almost never the problem when memory runs out?
2. Can a process go from Blocked/Swapped directly to Terminated? When?
3. The lecture says thread states (L10) use the five-state model, not seven. Why might the extra swap states be less relevant for threads?
