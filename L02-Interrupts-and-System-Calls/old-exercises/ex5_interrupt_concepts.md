# Lecture 2 — Exercise 5: Interrupt & Mode Concepts (pen and paper)

Answer without looking at the notes first, then check.

## 1. Polling vs. interrupts

A program asks a disk for data that takes 8 ms to arrive. The CPU can execute ~3 billion instructions per second.

a. Roughly how many instructions could the CPU have run while waiting?
b. Describe what the CPU does in a **polling** design vs. an **interrupt** design during those 8 ms.
c. Give one situation where polling might actually be the better choice. (Hint: what if the data arrives in nanoseconds?)

## 2. Classify the interrupt source

For each, say **Program**, **Timer**, **I/O**, or **Hardware failure**, and whether it is a *trap* (software-generated) or a hardware interrupt:

1. A process executes `x = 5 / 0;`
2. The network card finishes receiving a packet.
3. The OS's time slice for the current process runs out.
4. The UPS signals that wall power was lost.
5. A program calls `read()`.
6. A program dereferences a `NULL` pointer.

## 3. Priority and nesting

Interrupts L (low), M (medium), H (high) arrive as follows, with nested handling allowed only for strictly higher priority:

- t=0: user program running
- t=1: L arrives
- t=2: H arrives
- t=3: M arrives
- H's handler takes 3 units, M's takes 2, L's takes 4 (total work).

Draw a timeline of what is running from t=0 until the user program resumes. When does the user program resume?

Now redo it assuming **all handlers disable interrupts** (purely sequential, pending interrupts served by priority).

## 4. Save and restore

a. What exactly must be saved when an interrupt occurs, and where is it stored?
b. Why "(almost) as if the interruption never took place"? What might be observably different to the program afterward?
c. After the handler finishes, the OS might resume a *different* program. What is that called, and why is it possible?

## 5. Dual-mode operation

a. What is the mode bit? What is its value at boot, when a user program runs, and during a system call?
b. Name two instructions/operations that should be privileged and explain what could go wrong if user programs could run them.
c. Give the argument *against* dual mode (what does it cost?) and why we accept that cost.

## 6. Order the steps

Put these system-call steps in the correct order:

- (A) The OS runs the handler matching the identifier.
- (B) The user program pushes arguments on the stack.
- (C) The system call returns control to the user program.
- (D) The system call wrapper places its identifier in a register.
- (E) The trap instruction is executed.
- (F) Control exits the kernel back to the wrapper in user mode.
- (G) The user program calls the system call wrapper.
- (H) The OS examines the identifier in the designated location.

## 7. Short answer

a. Is `printf` a system call? Is `malloc`? Is `read`? Justify each (you can check with `strace` from Exercise 4).
b. Why is `read` allowed to return fewer bytes than requested? List two situations.
c. The lecture code does `read(fd, buffer, buf_size - 1)`. What bug happens if you use `buf_size` instead?
