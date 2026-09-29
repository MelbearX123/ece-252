/*
 * Lecture 10 — Exercise 3: Speed of Execution — Parallel Sum Timing
 *
 * GOAL
 *   Measure the "speed of execution" benefit of threads on a multi-core
 *   machine, and compare the programming effort to the fork + shared
 *   memory version from Lecture 9 Exercise 5.
 *
 * SPEC
 *   ./ex3 <n> <max_threads>
 *   - Allocate and fill an array of n ints (use n = 200,000,000 if you have
 *     ~1 GB free; otherwise 50,000,000). Fill ONCE, before timing.
 *   - For t in 1, 2, 4, ..., max_threads:
 *       * start = clock_gettime(CLOCK_MONOTONIC)
 *       * create t threads; each sums its slice into its OWN slot of a
 *         long partial[t] array (passed via its params struct) — NO shared
 *         total (see ex4 for why).
 *       * join all, add the partials
 *       * stop the clock; print  "threads=<t> sum=<s> time=<ms> speedup=<x>"
 *   - All sums must be identical.
 *
 * QUESTIONS
 *   1. `nproc` tells you how many cores you have. Where does speedup stop
 *      improving? Why can't it exceed the core count for this workload?
 *   2. Summing is memory-bound (the CPU waits on RAM more than it computes).
 *      Try making each element's work heavier (e.g. sum of sqrt(data[i])),
 *      link with -lm. Does the speedup curve improve? Why?
 *   3. Notice there's no shmget, no fork, no wait-status decoding — the
 *      threads just share `data`. Which of the lecture's 5 motivations is
 *      this?
 *   4. FALSE SHARING (stretch): replace partial[] with every thread doing
 *      partial[k] += data[i] inside the loop (instead of a local variable,
 *      then one write at the end). Through a volatile pointer, it can get
 *      dramatically slower even though no two threads touch the same
 *      element. Look up "false sharing" and cache lines.
 *
 * COMPILE
 *   gcc -Wall -Wextra -O2 -pthread ex3_parallel_sum_timing.c -o ex3
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    const int *data;
    size_t start;
    size_t end;
    long *out;          /* where to write this thread's partial sum */
} sum_args_t;

void *sum_worker( void *arg );

static double now_ms( void ) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
}

int main( int argc, char **argv ) {
    /* TODO */
    (void) now_ms;
    return 0;
}

void *sum_worker( void *arg ) { /* TODO */ return NULL; }
