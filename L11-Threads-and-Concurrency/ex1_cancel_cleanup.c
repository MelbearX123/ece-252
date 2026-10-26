/*
 * Lecture 11 — Exercise 1: Cancel a Thread Without Leaking
 *
 * GOAL
 *   Cancel a running thread from main, and make sure the memory the
 *   thread allocated still gets freed when it's cancelled.
 *
 * WHAT THE PROGRAM SHOULD DO
 *   - main starts one thread running worker(), lets it run for about
 *     3 seconds, then cancels it.
 *   - worker() allocates a buffer on the heap, then loops forever doing
 *     "work": each loop it prints a message and sleeps for 1 second.
 *   - main waits for the cancelled thread to really be gone, then prints
 *     whether the thread was cancelled (check what join gives you back).
 *
 * STEPS
 *   1. Get it working WITHOUT a cleanup handler first. Run it under
 *      valgrind (or compile with -fsanitize=address). You should see the
 *      buffer reported as leaked. Why does it leak?
 *   2. Add a cleanup handler so the buffer is freed even when the thread
 *      is cancelled. Run the leak check again: it should be clean.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g -pthread ex1_cancel_cleanup.c -o ex1
 *   valgrind --leak-check=full ./ex1
 *     or
 *   gcc -Wall -Wextra -g -pthread -fsanitize=address ex1_cancel_cleanup.c -o ex1
 *   ./ex1
 *
 * QUESTIONS
 *   1. Your worker never calls pthread_testcancel, yet it still stops.
 *      Where exactly does it stop, and why?
 *   2. Why would asynchronous cancellation be risky in this worker?
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *worker( void *arg );
void handler(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

int main( void ) {
    /* TODO */
    pthread_t t;
    pthread_create(&t, NULL, worker, NULL);\
    sleep(3); 
    pthread_cancel(t);

    void *result;
    pthread_join(t, &result);
    printf("thread was cancelled: %s\n", result == PTHREAD_CANCELED ? "yes" : "no");
    return 0;
}

void *worker( void *arg ) {
    /* TODO */
    int *buf = malloc(sizeof(int));
    pthread_cleanup_push(handler, buf);
    while (1) {
        printf("hi this is message\n");
        sleep(1);
    }
    pthread_cleanup_pop(1);
    pthread_exit(buf);
}
