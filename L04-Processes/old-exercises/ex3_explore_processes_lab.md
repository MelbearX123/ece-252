# Lecture 4 — Exercise 3: Exploring Real Processes on Linux

Linux exposes much of each process's PCB-like information through `ps` and the `/proc` file system ("everything is a file"). Run these on ecelinux or WSL and answer the questions.

## Part A — Who's running?

1. `ps` — only processes in your terminal. What are they? Which one is your shell?
2. `ps -ef | head -20` — all processes. What is PID 1 called on your system (`init`, `systemd`, …)? Who is its parent (PPID)?
3. `ps -eo pid,ppid,user,stat,cmd | less` — find your own shell. Follow its PPID upward (look up the parent, then its parent…) until you reach PID 1. Write down the chain. Compare with the tree in Lecture 5's notes (`init → sshd → bash` or `login → bash`).
4. `pstree -p $$` and `pstree -p | head -40` — the same thing drawn as a tree. (`$$` is the shell's own PID.)

## Part B — Daemons

1. List a few processes whose names end in `d` (e.g. `ps -ef | grep -E 'd$'`, or look for `sshd`, `systemd-journald`, `cron`). These are daemons.
2. What is their TTY column in `ps -ef`? Why does that make sense for background processes?

## Part C — The PCB, as exposed by `/proc`

Pick your shell's PID (`echo $$`) and run `cat /proc/$$/status`. Map lines of that output to the PCB fields from the lecture:

| PCB field | Line(s) in `/proc/<pid>/status` or other `/proc/<pid>/` file |
|-----------|-------------------------------------------------------------|
| Identifier | |
| State | |
| Priority | (hint: `cat /proc/$$/stat`, fields 18–19, or `ps -o pid,pri,ni -p $$`) |
| Memory pointers / usage | |
| I/O status (open files) | (hint: `ls -l /proc/$$/fd`) |
| Accounting info | (hint: `ps -o pid,time,etime -p $$`) |
| Parent | |

Why can't you see the saved **program counter** and **registers** here? (When are they stored in the PCB?)

## Part D — States in the wild

`ps` STAT codes: `R` running/runnable, `S` sleeping (interruptible — blocked waiting for an event), `D` uninterruptible sleep (blocked, usually on disk I/O), `T` stopped, `Z` zombie.

1. `ps -eo stat | sort | uniq -c` — which state are almost all processes in? Why does that make sense on a mostly idle machine? Which five-state model state does `S` correspond to?
2. Run `yes > /dev/null &` then `ps -o pid,stat,cmd`. What state is `yes` in? Why? Kill it with `kill %1`.
3. Run `sleep 100 &`. What state is `sleep` in? Why?
4. Run `cat` (with no arguments — it waits for keyboard input). In another terminal, find it with `ps -eo pid,stat,cmd | grep cat`. What state? Which event would unblock it?
5. Why does `R` in `ps` not distinguish between the lecture's **Ready** and **Running** states?

## Part E — `top`

Run `top` (press `q` to quit). Identify: total number of tasks and how many are running/sleeping/stopped/zombie, the process using the most CPU, and the `PR`/`NI` columns (priority). Press `Shift+P` / `Shift+M` to sort by CPU / memory.
