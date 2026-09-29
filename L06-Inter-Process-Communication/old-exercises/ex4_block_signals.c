/*
 * Lecture 6 — Exercise 4: Critical Sections with sigprocmask
 *
 * GOAL
 *   Block a signal during a section that must not be interrupted, observe
 *   that it stays PENDING, and restore the previous mask afterward.
 *
 * SCENARIO
 *   The program "saves a document" by writing 10 lines to save.txt with a
 *   1-second pause between lines. If Ctrl-C kills it halfway, the file is
 *   corrupt. Protect the save.
 *
 * SPEC
 *   ./ex4
 *   - Install a SIGINT handler that sets a flag (and write()s "[SIGINT]").
 *   - Build a sigset_t containing SIGINT using sigemptyset + sigaddset.
 *   - Print "Saving... Ctrl-C is blocked".
 *   - sigprocmask(SIG_BLOCK, &set, &previous).
 *   - Write the 10 lines, 1s apart. Press Ctrl-C several times during this.
 *   - Before unblocking, check whether SIGINT is pending:
 *       sigset_t pend; sigpending(&pend); sigismember(&pend, SIGINT)
 *     and print "SIGINT is pending" / "nothing pending".
 *   - sigprocmask(SIG_SETMASK, &previous, NULL). Print "Save complete".
 *     The handler should run right about now — exactly ONCE even if you
 *     pressed Ctrl-C five times.
 *   - If the flag is set, print "Interrupted after save, exiting" and exit.
 *
 * PART B
 *   Use sigfillset to block EVERYTHING during the save. Now try, from
 *   another terminal: kill -TERM <pid>, kill -STOP <pid> / kill -CONT <pid>,
 *   and kill -9 <pid>. Which ones get through, and why?
 *
 * PART C
 *   Use the old_set argument to print the mask before and after (loop over
 *   signals 1..31 with sigismember) to prove it was restored exactly.
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex4_block_signals.c -o ex4
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t interrupted = 0;

void on_sigint( int signo ) {
    /* TODO */
}

int main( void ) {
    /* TODO */
    return 0;
}
