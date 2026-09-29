/*
 * Lecture 10 — Exercise 7: Is Thread Creation Really ~10x Faster?
 *
 * GOAL
 *   Verify the lecture's performance claims yourself instead of taking
 *   them on faith.
 *
 * SPEC
 *   ./ex7 <count>          (try 10000)
 *   Measure with clock_gettime(CLOCK_MONOTONIC):
 *
 *   T1 processes: for i in 0..count-1: fork(); child does _exit(0)
 *                 immediately; parent waitpid()s it right away.
 *   T2 threads:   for i in 0..count-1: pthread_create a thread whose
 *                 function just returns NULL; pthread_join it right away.
 *   T3 processes with a big heap: first malloc and TOUCH (memset) 200 MB in
 *                 the parent, then repeat T1. Does fork get slower? Why?
 *                 (It must duplicate the page tables for that memory; look
 *                  up copy-on-write for why it's not even slower.)
 *   T4 threads with the same 200 MB allocated — does it matter?
 *
 *   Print for each: total ms, and microseconds per create+cleanup.
 *   Print the ratio T1/T2 and T3/T4.
 *
 * QUESTIONS
 *   1. Is your ratio close to the lecture's "on the order of ten times"?
 *      Modern Linux makes fork quite fast — what did you measure?
 *   2. Why does the big heap affect fork but not pthread_create?
 *   3. The lecture also claims switching between threads is cheaper than
 *      between processes. Why? (What doesn't need to change when switching
 *      between two threads of the same process? Think address space.)
 *   4. Given these numbers, when would you still choose processes over
 *      threads? (Revisit the drawbacks in the README.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -O2 -pthread ex7_creation_cost.c -o ex7
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>

static double now_ms( void ) {
    struct timespec ts;
    clock_gettime( CLOCK_MONOTONIC, &ts );
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1e6;
}

void *nothing( void *arg ) { (void) arg; return NULL; }

double time_processes( int count );
double time_threads( int count );

int main( int argc, char **argv ) {
    /* TODO */
    (void) now_ms;
    return 0;
}

double time_processes( int count ) { /* TODO */ return 0; }
double time_threads( int count ) { /* TODO */ return 0; }
