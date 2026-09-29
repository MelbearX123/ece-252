/*
 * Lecture 10 — Exercise 1: Hello, Threads (and the &i Bug)
 *
 * PART A — create and join
 *   ./ex1 A <n>
 *   - Create n threads (pthread_t array on the heap). Thread k prints
 *       "hello from thread <k> of <n>"
 *   - main joins all of them, then prints "all <n> threads done".
 *   - Check pthread_create's return value — it RETURNS an error number
 *     instead of setting errno; print it with strerror(rc).
 *   - Run with n = 8 several times. Is the order the same each time? Is
 *     "all done" ever printed before a thread's hello? Why not?
 *
 * PART B — the bug
 *   ./ex1 B <n>
 *   Pass the loop variable's ADDRESS:
 *       for (int i = 0; i < n; i++) pthread_create(&t[i], NULL, worker_b, &i);
 *   and in worker_b print *(int *)arg. Run many times with n = 8.
 *   You'll see duplicates and missing numbers (maybe even 8!). Explain
 *   EXACTLY why, using the Lecture 1 warning about stack memory that
 *   "sometimes does not outlive a particular iteration of a loop".
 *
 * PART C — two correct fixes
 *   ./ex1 C1 <n>  : allocate an int array ids[n] (heap, lives until after
 *                   the joins) and pass &ids[i].
 *   ./ex1 C2 <n>  : malloc one int per thread, pass it, and have the THREAD
 *                   free it. Who owns the memory in each fix?
 *   (There's also the cast trick (void *)(intptr_t)i — look it up; it works
 *    but hides the type. Which do you prefer?)
 *
 * PART D — too many joins
 *   Join the same thread twice. What does the second pthread_join return?
 *   (Lecture: it's an error — it may also be undefined behaviour; note what
 *    you observe but don't rely on it.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g -pthread ex1_hello_threads.c -o ex1
 *   Also try: gcc -g -fsanitize=thread -pthread ex1_hello_threads.c -o ex1_tsan
 *             ./ex1_tsan B 8        (ThreadSanitizer reports the bug!)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

void *worker_a( void *arg );
void *worker_b( void *arg );
void *worker_c2( void *arg );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

void *worker_a( void *arg ) { /* TODO */ return NULL; }
void *worker_b( void *arg ) { /* TODO */ return NULL; }
void *worker_c2( void *arg ) { /* TODO */ return NULL; }
