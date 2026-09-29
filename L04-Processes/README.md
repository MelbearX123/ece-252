# Lecture 4 — Processes

## 1. What Is a Process?

Early computers (and many embedded systems today) ran exactly one thing, with all resources to itself. Modern OSes run many programs concurrently, and to manage that complexity they use the **process** abstraction.

> A **process** is a **program in execution**.

It consists of:

1. **The instructions and data** of the program (the compiled executable).
2. **The current state** of the program (where it is, register values, variables…).
3. **The resources** needed to execute it (memory, open files, devices…).

A *program* is a passive file on disk; a *process* is an active running instance. **Two instances of the same program are two separate processes** — two Word windows, or two users running Firefox on the same server.

## 2. The Process Control Block (PCB)

The OS's data structure for each process. Created when the process is created, updated as it runs, thrown away once it has finished and been cleaned up. The kernel keeps all PCBs in some container (e.g. a list).

A PCB typically contains:

| Field | Contents |
|-------|----------|
| **Identifier** | Unique ID (usually an incrementing integer, reset at reboot). In UNIX: the **pid**. |
| **State** | Running, ready, blocked, … (see state models below). |
| **Priority** | Importance relative to other processes (for scheduling). |
| **Program counter** \* | Address of the next instruction to execute. |
| **Register data** \* | Saved values of the CPU registers ("context data"). |
| **Memory pointers** | Where the process's code and data are, plus memory the OS allocated on request. |
| **I/O status information** | Outstanding I/O requests, open files, assigned devices. |
| **Accounting information** | Resource usage (CPU time used, etc.). Optional but common. |

\* **"When needed"**: while the process is actually running, the PC and registers live *in the CPU*, and there's no need to keep copying them into the PCB. Only when the process is **suspended** (system call/trap, interrupt, or process switch) does the OS **save** them into the PCB so it can later resume exactly where it left off with the CPU exactly as it was.

### Context switch (P0 → P1 → P0)

1. P0 is executing.
2. An interrupt or system call occurs → OS takes over.
3. OS **saves P0's state into PCB0**.
4. OS **reloads P1's state from PCB1**.
5. P1 executes (P0 is idle).
6. Interrupt/system call → OS saves state into PCB1, reloads from PCB0.
7. P0 resumes.

The time spent in steps 3–4 and 6 is pure overhead — no user work gets done during a switch.

## 3. The Circle of Life

- **Creation**: OS creates a new PCB, initializes it (initial state, instruction pointer at the start of `main`, …), and adds it to its set of PCBs.
- **Destruction**: after termination and cleanup, the OS may collect some final data (accounting summary), then removes the PCB.

### Three ways a process is created

1. **System boot** — the OS starts initial processes. Some are **foreground** (visible, e.g. the login screen); some are **background**. The UNIX name for a background service process is a **daemon** (e.g. `sshd`, the Secure Shell Daemon that answers `ssh` connections).
2. **User request** — double-clicking an icon, typing a command.
3. **A process spawns another** — e.g. your email client launching a browser for a link, or a program splitting work into sub-processes (for parallelism or fault tolerance). The spawner is the **parent**, the new process the **child**.

### Four ways a process terminates

| # | Kind | Example |
|---|------|---------|
| 1 | **Normal exit** (voluntary) | Compiler finishes; you close your text editor. |
| 2 | **Error exit** (voluntary) | Program checks for write access to `/tmp`, doesn't have it, exits with an error code. Compiler asked to compile a nonexistent file. |
| 3 | **Fatal error** (involuntary) | Stack overflow, divide by zero. OS sends the error to the program; if it isn't handled, the process is terminated. |
| 4 | **Killed by another process** (involuntary) | User runs `kill` (UNIX) / "End Process" (Windows); a parent kills a stuck child. You need the *rights* to kill a process — normally only your own, unless you're an administrator. |

Killing a parent does **not** automatically kill its children in UNIX or Windows — they can outlive each other.

## 4. The Process Family Tree (UNIX)

- UNIX **maintains the parent–child hierarchy** (Windows does not).
- Every process has **exactly one parent** and zero or more children.
- A process and its descendants form a **process group**; some signals (like Ctrl-C) can be sent to a whole group, each member deciding how to react.
- The first process is **`init`** (pid 1; on some systems `systemd` or `launchd`). Walking up the tree from any process eventually reaches it — like `Object` in Java.
- In **Windows**, a parent gets a *handle* to its child, but can hand it to another process ("adoption"), so there's no true hierarchy. In UNIX a parent can't disinherit a child.

### Return codes, zombies, and orphans

- A terminating process produces a **return code** (like a function's return value). Conventionally **0 = success**, non-zero = some error. The parent and child must agree on what codes mean.
- When launched from a shell or icon, the code is usually ignored; in UNIX a **parent can collect** its child's code with `wait` (Lecture 5).

**Zombie**: a child that has **finished** but whose parent **hasn't collected its return value yet**. It is dead but not gone — its PCB entry still exists and it holds its resources until the parent **reaps** it with `wait`. Usually the parent waits right away, so zombies are brief; if the parent is slow or never waits, zombies accumulate.

**Orphan**: a child whose **parent died first**. In UNIX, orphans are automatically **adopted by `init`**, which waits on all its children (discarding the values) so they never linger as zombies. Sometimes orphaning is **intentional** — e.g. starting a daemon that should keep running in the background.

## 5. The Five-State Model

| State | Meaning |
|-------|---------|
| **New** | Just created (ID assigned, PCB made) but not yet admitted to run. Often not yet in memory. The OS may delay admitting it (e.g. to limit the number of concurrent processes). |
| **Ready** | Could run right now if the scheduler picked it. |
| **Running** | Actually executing on a CPU. |
| **Blocked** | Can't proceed until some event happens (I/O completes, user input arrives, a resource becomes available). Picking it would waste CPU time. |
| **Terminated** | Finished executing but not yet cleaned up (**zombie** until reaped). |

```
          Admit            Dispatch           Exit
  New ───────────▶ Ready ◀──────────▶ Running ─────▶ Terminated ──Reap──▶ (gone)
  ▲                 ▲      Suspend        │
Create              │                     │ Block
                    │                     ▼
                    └─────Unblock──── Blocked
```

**Eight transitions**:

| Transition | From → To | Cause |
|------------|-----------|-------|
| **Create** | — → New | Process is created. |
| **Admit** | New → Ready | OS accepts it into the set of processes that may run. |
| **Dispatch** | Ready → Running | Scheduler picks it. |
| **Suspend** | Running → Ready | It's paused (e.g. time slice used up, higher-priority process), but could still run. |
| **Block** | Running → Blocked | It requests something (I/O, resource) it can't get immediately. |
| **Unblock** | Blocked → Ready | The awaited event happens. Note: goes to **Ready**, not directly to Running. |
| **Exit** | Running → Terminated | Process finishes; return value becomes available. |
| **Reap** | Terminated → gone | Return value collected with `wait`; resources released. |

Not shown in the diagram: a process in **Ready** or **Blocked** can go directly to **Terminated** if it's **killed** (by the user or its parent), or if some OS policy kills children when the parent dies.

## 6. Swapping and the Seven-State Model

PCBs are small (a few KB), but a process's stack and heap can be **gigabytes**. If users run more processes than fit in RAM, the OS **swaps** parts of some processes out to disk. This is **very expensive** (disk is "seven eternities" slow from the CPU's view) and has to be undone (swapped back in) before the process can run — so it's done only when necessary.

The best victim is a **blocked** process (it can't run anyway). But a single "Swapped" state isn't enough:

1. What if all processes are Ready and we still need memory? We need to swap out a *ready* process.
2. If a swapped-out blocked process's event occurs, the OS needs to know that it's now runnable without guessing (swapping it in to check would be too costly).

So "swapped" is split into two, giving the **seven-state model**:

| State | Meaning |
|-------|---------|
| New, Ready, Running, Blocked, Terminated | As before |
| **Ready/Swapped** | Ready to run, but currently on disk. |
| **Blocked/Swapped** | Waiting for an event **and** on disk. |

New transitions:

- **Admit** now typically goes New → **Ready/Swapped** (new processes don't start in memory by default).
- **Swap Out**: Ready → Ready/Swapped, Blocked → Blocked/Swapped.
- **Swap In**: Ready/Swapped → Ready, (and Blocked/Swapped → Blocked if the OS expects the event soon).
- **Unblock (second one)**: Blocked/Swapped → Ready/Swapped — the event happened, the process can run, but it's still on disk.

As before, a killed process can go to Terminated from any state, in memory or on disk.

---

## New Functions Introduced

This lecture is **conceptual** — no new C functions. Commands mentioned:

| Command | Purpose |
|---------|---------|
| `ps` | List processes (with their pids). |
| `kill <pid>` | Terminate a process (by sending it a signal — see L05). |
| `ssh` / `sshd` | Example of a client and its daemon. |

Useful for the exercises: `ps -ef`, `ps -eo pid,ppid,stat,cmd`, `pstree -p`, `top`/`htop`, `/proc/<pid>/status`.

## Exercises

1. [ex1_state_tracing.md](ex1_state_tracing.md) — trace a process through the five-state model.
2. [ex2_ps_lab.md](ex2_ps_lab.md) — look at real processes with `ps` and `/proc`.

Older, longer exercises are in [old-exercises/](old-exercises/) for extra practice.
