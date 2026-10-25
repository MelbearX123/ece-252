/*
 * Lecture 10 — Exercise 2: A Race Condition
 *
 * GOAL
 *   See for yourself that threads share memory with no protection.
 *
 * WHAT THE PROGRAM SHOULD DO
 *   - Two threads both increase the shared global counter by 1,
 *     ITERATIONS times each.
 *   - After both threads are finished, main prints what the counter
 *     SHOULD be next to what it actually is.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g -pthread ex2_shared_counter_race.c -o ex2
 *   ./ex2        (run it several times)
 *
 * QUESTIONS
 *   1. Is the answer right? Is it the same every run?
 *   2. Change it to use 1 thread. Is it always right now? Why?
 *   3. Explain what goes wrong. (Hint: adding 1 to a variable is really
 *      three steps for the CPU. What if two threads interleave them?)
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define ITERATIONS 1000000

long counter = 0;

void *increment( void *arg );

int main( void ) {
    pthread_t *p_tids = malloc(sizeof(pthread_t) * 2);
    for (int i = 0; i < 2; i++) {
        pthread_create(p_tids+i, NULL, increment, NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(p_tids[i], NULL);
    }
    printf("Expected: %d, Actual: %ld\n", 2 * ITERATIONS, counter);
    free(p_tids);
    return 0;
}

void *increment( void *arg ) {
    for (int i = 0; i < ITERATIONS; i++) {
        counter++;
    }
    return NULL;
}
