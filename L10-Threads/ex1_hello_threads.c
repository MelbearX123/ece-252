/*
 * Lecture 10 — Exercise 1: Hello, Threads
 *
 * GOAL
 *   Start several threads, give each one its own number, and wait for
 *   all of them to finish.
 *
 * WHAT THE PROGRAM SHOULD DO
 *   - Start N threads that all run worker().
 *   - Each thread should know which one it is (0 to N-1) and print a
 *     greeting that includes its number.
 *   - main must not finish until every thread has finished. Once they
 *     have, main prints a final message.
 *
 * THINGS TO FIGURE OUT
 *   - worker() takes a single void * argument. How do you get a thread's
 *     number to it through that?
 *   - Wherever you store each thread's number, it has to stay valid
 *     until that thread has read it.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g -pthread ex1_hello_threads.c -o ex1
 *   ./ex1       (run it a few times: is the order always the same?)
 *
 * QUESTIONS
 *   1. Why can't you just pass every thread the address of the loop
 *      counter? Try it and see what numbers get printed.
 *   2. What happens if main returns without waiting for the threads?
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 4

struct thread_args {
    int num;
};

struct thread_res{
    int square;
};

void *worker( void *arg );

int main( void ) {
    pthread_t *p_tids = malloc(sizeof(pthread_t) * N);
    struct thread_args in_params[N];
    struct thread_res *out_res[N];
    for (int i = 0; i < N; i++) {
        in_params[i].num = i;
        pthread_create(p_tids+i, NULL, worker, in_params+i);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(p_tids[i], (void**)&out_res[i]);
    }

    int sum = 0;
    for (int i = 0; i < N; i++) {
        sum += out_res[i]->square;
        free(out_res[i]);
    }
    printf("Sum is: %d\n", sum);
    free(p_tids);
    return 0;
}

void *worker( void *arg ) {
    struct thread_args *p_in = arg;
    struct thread_res *p_out = malloc(sizeof(struct thread_res));

    p_out->square = (p_in->num) * (p_in->num);
    printf("Num: %d, Squared: %d\n", p_in->num, p_out->square);
    return (void *)p_out;
}
