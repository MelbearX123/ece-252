/*
 * Lecture 10 — Exercise 4: No Protection Between Threads (a Race)
 *
 * GOAL
 *   The lecture warns: "there is no protection between threads in the same
 *   process: so one thread can easily mess with the memory being used by
 *   another thread." Make that concrete. (The fix — coordination — is the
 *   next part of the course. Here you just observe and explain.)
 *
 * SPEC
 *   ./ex4 <nthreads> <iterations>
 *   - Global: long counter = 0;   (shared — globals are per-process)
 *   - Each thread runs:  for (i = 0; i < iterations; i++) counter++;
 *   - main creates nthreads threads, joins them all, prints:
 *       "expected <nthreads*iterations>, got <counter>, lost <diff>"
 *   - Run with 2 threads x 1,000,000 iterations, 10 times. Record results.
 *
 * TASKS
 *   1. Compile at -O0. Run. Wrong? Different each run?
 *   2. Compile at -O2. Run. What changes? (The compiler may turn the loop
 *      into counter += iterations — sometimes "fixing" it by accident, which
 *      is worse because the bug hides.) Make counter `volatile long` and
 *      run again at -O2.
 *   3. Run with 1 thread. Always correct? Why?
 *   4. Look at the assembly for counter++ (gcc -S -O0 ex4_shared_counter_race.c,
 *      open the .s file, find the load/add/store). Write a 6-step
 *      interleaving of two threads (T1 load, T2 load, T1 add, ...) that
 *      loses one increment.
 *   5. Replace the global with a LOCAL counter in each thread that is
 *      returned via pthread_exit (heap-allocated) and summed by main.
 *      Always correct now? Why? Which "Per thread" row of the README table
 *      is protecting you?
 *   6. Run with -fsanitize=thread (compile + run) for the original version.
 *      Read the data race report.
 *
 * COMPILE
 *   gcc -Wall -Wextra -O0 -g -pthread ex4_shared_counter_race.c -o ex4
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long counter = 0;

void *increment( void *arg );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

void *increment( void *arg ) { /* TODO */ return NULL; }
