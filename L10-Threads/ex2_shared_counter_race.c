/*
 * Lecture 10 — Exercise 2: A Race Condition
 *
 * GOAL
 *   See that threads share memory with no protection.
 *
 * SPEC
 *   - Global: long counter = 0;
 *   - increment(): loop ITERATIONS times doing counter++.
 *   - main: create 2 threads running increment, join both, print
 *       "expected <2 * ITERATIONS>, got <counter>"
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g -pthread ex2_shared_counter_race.c -o ex2
 *   ./ex2        (run it several times)
 *
 * QUESTION
 *   Why is the answer usually wrong, and different each run?
 *   (Hint: counter++ is really load, add, store.)
 */

#include <stdio.h>
#include <pthread.h>

#define ITERATIONS 1000000

long counter = 0;

void *increment( void *arg );

int main( void ) {
    /* TODO */
    return 0;
}

void *increment( void *arg ) {
    /* TODO */
    return NULL;
}
