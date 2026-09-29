/*
 * Lecture 10 — Exercise 2: Struct Arguments In, Struct Results Out
 *
 * GOAL
 *   The lecture's recommended pattern: one struct type for parameters, one
 *   for results; the thread mallocs its result, pthread_exits with it, and
 *   the joiner collects it via void ** and frees it.
 *
 * TYPES
 *   typedef struct { const double *data; size_t start; size_t end; int id; } params_t;
 *   typedef struct { int id; double min; double max; double sum; size_t count; } result_t;
 *
 * SPEC
 *   ./ex2 <n> <nthreads>
 *   - Fill a heap array of n doubles with rand() / (double) RAND_MAX * 100.
 *     Seed with a fixed value so runs are reproducible.
 *   - Create nthreads threads; thread k gets params for its slice
 *     [k*n/nthreads, (k+1)*n/nthreads). Store the params in an ARRAY of
 *     params_t (not a single reused variable — why?).
 *   - stats_worker():
 *       * cast void * -> params_t *
 *       * compute min/max/sum/count of its slice
 *       * result_t *r = malloc(...); fill it; pthread_exit(r)   (Part A)
 *   - main: for each thread, pthread_join(t[k], &vr); cast to result_t *;
 *     print "thread <id>: count=.. min=.. max=.. mean=.."; combine into
 *     overall min/max/mean; free(vr).
 *   - Verify the combined results against a single-threaded pass.
 *   - valgrind --leak-check=full: zero leaks.
 *
 * PART B — the wrong way to return
 *   Write stats_worker_bad() that returns the ADDRESS OF A LOCAL result_t
 *   (return &local;). Compile — read the warning. Run — what does main
 *   print? Explain in terms of each thread having its own stack that
 *   disappears when the thread exits.
 *
 * PART C — return vs pthread_exit
 *   Change stats_worker to end with `return r;` instead of pthread_exit(r).
 *   Any difference in behaviour? (Lecture: returning normally terminates the
 *   thread just the same.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g -pthread ex2_struct_args_and_results.c -o ex2
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    const double *data;
    size_t start;
    size_t end;
    int id;
} params_t;

typedef struct {
    int id;
    double min;
    double max;
    double sum;
    size_t count;
} result_t;

void *stats_worker( void *arg );
void *stats_worker_bad( void *arg );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

void *stats_worker( void *arg ) { /* TODO */ return NULL; }
void *stats_worker_bad( void *arg ) { /* TODO */ return NULL; }
