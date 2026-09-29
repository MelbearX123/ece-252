/*
 * Lecture 6 — Exercise 3: Signal Ping-Pong
 *
 * GOAL
 *   Use kill() to send signals between a parent and child, and pause() to
 *   wait for them. Then discover the race condition in the naive version
 *   and fix it with sigprocmask.
 *
 * SPEC
 *   ./ex3 <rounds>
 *   - Install handlers for SIGUSR1 and SIGUSR2 (they just set a flag).
 *     Install them BEFORE fork so the child inherits them.
 *   - Fork.
 *   - Parent: for each round: kill(child, SIGUSR1); print "[parent] ping <i>";
 *             wait for SIGUSR2 using pause().
 *   - Child:  for each round: wait for SIGUSR1 using pause();
 *             print "[child] pong <i>"; kill(getppid(), SIGUSR2).
 *   - After all rounds, parent sends SIGTERM to the child and wait()s.
 *
 * PART A — naive version
 *   Implement it as described. Run with rounds = 5, then 100000.
 *   It may hang forever at a large round count. WHY?
 *   (Think: what if the signal arrives AFTER you check the flag but BEFORE
 *    you call pause()? pause() then waits for a signal that already came.)
 *
 * PART B — fix it
 *   Block SIGUSR1/SIGUSR2 with sigprocmask(SIG_BLOCK, ...) before fork.
 *   Then, instead of "check flag; pause()", use
 *       while (!flag) sigsuspend(&old_mask);
 *   sigsuspend atomically swaps in a mask (one where the signal is NOT
 *   blocked) and sleeps — so there's no window for the signal to slip
 *   through. (sigsuspend is not in the lecture; read `man 2 sigsuspend`.)
 *   Confirm 100000 rounds completes.
 *
 * PART C — lost signals
 *   Change the parent to send 1000 SIGUSR1s in a tight loop without waiting
 *   for replies; the child just counts how many times its handler runs.
 *   The child keeps SIGUSR1 blocked, sleeps 1s so they all pile up as
 *   pending, then unblocks and prints the count after a moment. Why is it
 *   far less than 1000? (Lecture: a signal sent several times while pending
 *   may be delivered only once.) What does that tell you about using
 *   signals as a counter or data channel?
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex3_signal_ping_pong.c -o ex3
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static volatile sig_atomic_t got_usr1 = 0;
static volatile sig_atomic_t got_usr2 = 0;

void handler( int signo ) {
    /* TODO */
}

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}
