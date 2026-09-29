# Lecture 6 — Exercise 8: IPC Concepts

## 1. Classify the sync/async combination

For each scenario, name the send mode and receive mode, and which of the four combinations it is.

1. You text a friend and keep scrolling; your friend's phone buzzes and they check it whenever.
2. A phone call: you wait for them to pick up; they wait for you to talk.
3. The Lecture 6 message-queue example (parent `msgsnd` with flag 0, child `msgrcv` with flag 0), assuming the queue isn't full.
4. A consumer calls `msgrcv(..., IPC_NOWAIT)` and does other work if nothing is there; the producer calls `msgsnd(..., 0)` on a nearly empty queue.
5. The producer calls `msgsnd(..., 0)` on a **full** queue. What's the send mode *in that moment*?
6. Your file-system IPC program from Exercise 1.

## 2. Producer–consumer

Identify the producer, consumer, message, and agreed format in:

a. `cat access.log | grep 404`
b. A web browser loading a page.
c. A compiler writing `a.o` that the linker later reads.

## 3. Choose the mechanism

For each, choose **file system**, **signals**, or **message queue**, and justify in one or two sentences.

1. Tell a daemon to reload its configuration file.
2. A logging process that must keep messages even if the machine reboots before they're processed.
3. Twenty worker processes each sending small result structs back to one coordinator.
4. Two programs written by different companies that know nothing about each other's pids.
5. Notify a parent that a child has finished (hint: which signal is sent automatically?).
6. Urgent jobs should be processed before normal ones.

## 4. Signals — true or false (correct the false ones)

1. A signal can carry an integer payload chosen by the sender.
2. `kill` always terminates the target process.
3. If `SIGINT` is sent five times while blocked, the handler will run five times when it's unblocked.
4. You can install a handler for `SIGKILL` to save data before dying.
5. `printf` is safe to call inside a signal handler as long as the program is single-threaded.
6. `raise(SIGTERM)` is equivalent to `kill(getpid(), SIGTERM)`.
7. `kill(pid, 0)` returning 0 guarantees the process you meant is still running.
8. A signal sent to a process that is not currently running is lost.

## 5. Message queues

1. Why must every message struct begin with `long mtype`?
2. A message is `struct { long mtype; int a; double b; char name[20]; }`. What `nbytes` should you pass to `msgsnd`? Write the expression.
3. A queue contains messages with types (in order): 5, 2, 7, 2, 1, 9. What does each call return?
   - `msgrcv(..., type = 0, ...)`
   - `msgrcv(..., type = 2, ...)`
   - `msgrcv(..., type = -6, ...)`
   - `msgrcv(..., type = 3, IPC_NOWAIT)`
4. What are the three strategies when a queue is full? Which one does `msgsnd` with flag 0 implement? With `IPC_NOWAIT`?
5. Process A creates a queue with `IPC_PRIVATE` and prints the id. Can unrelated process B use that id? What are the trade-offs versus `ftok`?
6. Your program crashes before `msgctl(IPC_RMID)`. What happens to the queue? How do you find and remove it?
