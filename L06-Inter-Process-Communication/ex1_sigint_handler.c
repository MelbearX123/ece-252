/*
 * Lecture 6 — Exercise 1: Catch Ctrl-C
 *
 * GOAL
 *   Register a signal handler with signal() and use the
 *   "set a flag in the handler, act on it in main" pattern.
 *
 * SPEC
 *   - In main, register handler() for SIGINT.
 *   - Loop while quit == 0: print "working..." and sleep(1).
 *   - handler() only sets quit = 1 (no printf in a handler!).
 *   - After the loop, print "Caught Ctrl-C, exiting cleanly." and return 0.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_sigint_handler.c -o ex1
 *   ./ex1        (press Ctrl-C)
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t quit = 0;

void handler( int signo ) {
    /* TODO */
}

int main( void ) {
    /* TODO */
    return 0;
}
