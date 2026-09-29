/*
 * Lecture 10 — Exercise 6: Cancelling the Upload (Thread Cancellation)
 *
 * SCENARIO (from the lecture)
 *   A file-transfer program runs the "upload" in a background thread so the
 *   UI (main thread) stays responsive and the user can click Cancel.
 *
 * SPEC
 *   ./ex6
 *   - upload_thread(): "uploads" 100 chunks. For each chunk:
 *       * simulate work with a busy loop (~50 ms of computation — NOT
 *         sleep; see Part B for why)
 *       * update a shared progress variable (volatile int progress)
 *       * call pthread_testcancel()   <- deferred cancellation point
 *     When done, return a heap-allocated int with the number of chunks.
 *   - main ("UI thread"): creates the upload thread, then loops reading
 *     lines from stdin:
 *       "p" -> print "progress: <n>%"
 *       "c" -> pthread_cancel(upload), then join it
 *     When joined, if retval == PTHREAD_CANCELED print "upload cancelled at
 *     <n>%", otherwise print "upload finished: <chunks> chunks" (and free).
 *     If the upload finishes on its own, the next command should notice
 *     (hint: a volatile int done flag the thread sets).
 *
 * PART B — cancellation points
 *   1. Remove pthread_testcancel() from the loop. Press c. What happens?
 *      Why? (Deferred cancellation only takes effect at cancellation
 *      points; a pure computation loop has none.)
 *   2. Put back the busy loop but replace it with usleep(50000). Remove
 *      pthread_testcancel. Does cancel work now? Why? (Look up which
 *      functions are cancellation points: man 7 pthreads.)
 *
 * PART C — cleanup on cancel
 *   The lecture: with deferred cancellation "the thread is responsible for
 *   cleaning up after itself". Make the upload thread malloc a 1 MB "chunk
 *   buffer" at start. If cancelled, it leaks (check with valgrind). Fix it
 *   with pthread_cleanup_push(free, buf) / pthread_cleanup_pop(1) around
 *   the loop (look them up).
 *
 * COMPILE
 *   gcc -Wall -Wextra -g -pthread ex6_cancellation.c -o ex6
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define CHUNKS 100

static volatile int progress = 0;
static volatile int done = 0;

/* Burn roughly `ms` milliseconds of CPU without any cancellation points. */
static void busy_work( int ms ) {
    struct timespec start, now;
    clock_gettime( CLOCK_MONOTONIC, &start );
    do {
        clock_gettime( CLOCK_MONOTONIC, &now );
    } while ( ( now.tv_sec - start.tv_sec ) * 1000 +
              ( now.tv_nsec - start.tv_nsec ) / 1000000 < ms );
}

void *upload_thread( void *arg );

int main( void ) {
    /* TODO */
    (void) busy_work;
    return 0;
}

void *upload_thread( void *arg ) { /* TODO */ return NULL; }
