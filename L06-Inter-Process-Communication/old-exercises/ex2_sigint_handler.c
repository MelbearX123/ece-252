/*
 * Lecture 6 — Exercise 2: Handling Ctrl-C Safely
 *
 * GOAL
 *   Register a signal handler with signal(), obey the reentrancy rules, and
 *   use the "set a flag, handle it in main" pattern.
 *
 * SPEC
 *   ./ex2
 *   - Allocate a heap buffer and open "progress.log" for writing.
 *   - Main loop: once per second, fprintf "working... step <n>" to the log
 *     and printf a dot.
 *   - First Ctrl-C:  print (from MAIN, not the handler)
 *       "\nCtrl-C received (1/3). Press twice more to quit."
 *   - Second:        "(2/3)"
 *   - Third:         exit the loop, then CLEAN UP: write "shutdown clean" to
 *                    the log, fclose, free, print "Bye." and return 0.
 *   - SIGTERM (from `kill <pid>` in another terminal) should trigger the
 *     same clean shutdown immediately. Use ONE handler for both signals and
 *     tell them apart with its int parameter.
 *
 * RULES FOR THE HANDLER (lecture: reentrancy / async-signal-safe)
 *   - NO printf, malloc, free, fopen, fprintf in the handler.
 *   - The handler may only: update  volatile sig_atomic_t  globals, and
 *     (optionally) call write(STDOUT_FILENO, "!", 1) which IS safe.
 *
 * EXPERIMENTS
 *   1. Replace the handler with SIG_IGN for SIGINT. What happens on Ctrl-C?
 *      How do you stop the program now? (Ctrl-\ sends SIGQUIT...)
 *   2. Why doesn't `kill -9 <pid>` produce "shutdown clean" in the log?
 *   3. The call sleep(1) returns EARLY when a signal arrives — print its
 *      return value to see this. Why does that matter for loops that
 *      "sleep then check a flag"?
 *   4. Why must the flag be `volatile`? (What might the optimizer do with
 *      `while (!quit)` if it isn't? Compile with -O2 and try without it.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex2_sigint_handler.c -o ex2
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t int_count = 0;
static volatile sig_atomic_t term_requested = 0;

void handler( int signo ) {
    /* TODO */
}

int main( void ) {
    /* TODO */
    return 0;
}
