/*
 * Lecture 12 — Exercise 2: Fix the Race with Test-and-Set
 *
 * GOAL
 *   Remember Lecture 10's shared counter, where two threads incrementing
 *   one variable got the wrong total? Fix it with the lecture's
 *   test-and-set approach to mutual exclusion.
 *
 * WHAT THE PROGRAM SHOULD DO
 *   - Two threads each add 1 to the shared counter ITERATIONS times.
 *   - main waits for both, then prints the expected total next to the
 *     actual counter.
 *   - The answer must be right on EVERY run.
 *
 * WHAT'S PROVIDED
 *   test_and_set() and release() below behave exactly like the lecture's
 *   version: test_and_set() returns true if you got the lock, false if
 *   someone else has it. Use them; don't change them.
 *
 * THINGS TO FIGURE OUT
 *   - Which line(s) are the critical section?
 *   - Where exactly do you acquire the lock and release it?
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g -pthread ex2_test_and_set.c -o ex2
 *   time ./ex2
 *
 * QUESTIONS
 *   1. Run it with and without the lock and compare `time`. Why is the
 *      locked version so much slower?
 *   2. What is the waiting thread doing while the other one holds the
 *      lock? What's this called, and why is it wasteful?
 *   3. Would it be faster to lock once around the whole loop instead of
 *      around each increment? What would you lose by doing that?
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <pthread.h>

#define ITERATIONS 1000000

long counter = 0;
atomic_flag busy = ATOMIC_FLAG_INIT;

/* The lecture's test-and-set: atomically "if free, take it".
   Returns true if you got it, false if it was already taken. */
bool test_and_set( atomic_flag *flag ) {
    return !atomic_flag_test_and_set( flag );
}

/* The lecture's "busy = 0": give the lock back. */
void release( atomic_flag *flag ) {
    atomic_flag_clear( flag );
}

void *increment( void *arg );

int main( void ) {
    /* TODO */
    return 0;
}

void *increment( void *arg ) {
    /* TODO */
    return NULL;
}
