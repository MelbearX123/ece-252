# Lecture 5 — Exercise 6: Signals, Jobs, and `screen` from the Command Line

## Part A — Background jobs with `&`

1. `sleep 60 &` — note the `[job] pid` the shell prints.
2. `jobs` and `ps -o pid,ppid,stat,cmd` — confirm `sleep`'s parent is your shell (`echo $$`).
3. Run `sleep 3 &` and keep typing other commands. When does `[n]+ Done` appear?
4. `(for i in 1 2 3; do echo tick $i; sleep 1; done) &` — the output still lands in your terminal. Now redirect it: `(...) > ticks.log &`.
5. `fg` brings a background job to the foreground; `Ctrl-Z` then `bg` sends a foreground job to the background. Try it with `sleep 60`. What signal does `Ctrl-Z` send? (Hint: `SIGTSTP`.)

## Part B — Sending signals

Start `sleep 1000 &` and get its pid. For each command below, predict then check what happens using `jobs` / `ps` and the shell's report. After each one, use `echo $?` right after `wait <pid>` to see the exit status (the shell reports signal deaths as `128 + signal number`).

1. `kill <pid>` — which signal was sent? Exit status?
2. Start another; `kill -INT <pid>` — same as pressing Ctrl-C.
3. Start another; `kill -STOP <pid>` — check STAT in `ps` (should be `T`). Then `kill -CONT <pid>`. Then `kill -9 <pid>`.
4. `kill -l` — list all signal names and numbers on this machine. Find `SIGCHLD`, `SIGCONT`, `SIGSTOP`. Do the numbers match the first, second, or third value in the lecture table?
5. `kill -0 <pid>` on a live process and on a dead one; check `$?` each time. What is this useful for? (Previewed in Lecture 6.)
6. Try `kill -9 1`. What happens and why?

## Part C — Signals that come from the program itself (synchronous)

Compile and run each, then `echo $?`:

```c
int main( void ) { int *p = NULL; return *p; }            /* SIGSEGV */
```
```c
int main( int argc, char **argv ) { return 10 / ( argc - 1 ); }   /* run with no args: SIGFPE */
```
```c
#include <stdlib.h>
int main( void ) { abort(); }                              /* SIGABRT */
```

Which of these are synchronous and which are asynchronous signals? What exit status does the shell report for each?

## Part D — Use your Exercise 2 program

Run `./ex2 sleep 100`. From another terminal, send it `SIGTERM`, `SIGINT`, `SIGKILL` (one run each). Confirm your program's "killed by signal N" message matches `kill -l`.

## Part E — `screen`

1. `screen` → you're in a new shell inside screen. Run `top`.
2. Detach with `Ctrl-a d`. You're back at the original shell; `top` is still running.
3. `screen -ls` — list sessions. `screen -r` — reattach.
4. Bonus: detach, log out of ssh entirely, log back in, `screen -r`. Still there? Why doesn't logging out kill it? (Think SIGHUP and which process is `top`'s parent.)
