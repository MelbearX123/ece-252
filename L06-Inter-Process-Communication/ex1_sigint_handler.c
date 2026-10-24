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
    //handler should not do anything except set a value
    //and let main handle it
    quit = 1;
}

int main( void ) {
    /* TODO */
    signal(SIGINT, handler); //now on the signal, run handler instead of killing
    //instead of a function, can also do
    //signal(SIGINT, SIG_IGN) ignores signal
    //signal(SIGINT, SIG_DFL) go back to default


    while (!quit) {
        printf("working...\n");
        sleep(1);
    }
    printf("Cntrl-C caught, cleaning up\n");
    return 0;

    /*
    other notes
    sigprocmask blocks signals for a bit:
    sigset_t set, previous;
    sigemptyset( &set );                         // start with an empty set
    sigaddset( &set, SIGINT );                   // put SIGINT in it

    sigprocmask( SIG_BLOCK, &set, &previous );   // block SIGINT (save the old mask)
    //important work — Ctrl-C can't interrupt this
    sigprocmask( SIG_SETMASK, &previous, NULL ); // restore — a pending Ctrl-C hits now
    */
}
