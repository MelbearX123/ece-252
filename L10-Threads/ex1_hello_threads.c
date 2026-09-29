/*
 * Lecture 10 — Exercise 1: Hello, Threads
 *
 * GOAL
 *   Create and join threads, and pass each one an argument.
 *
 * SPEC
 *   - Make an array ids[N] with ids[i] = i.
 *   - Create N threads with pthread_create, passing &ids[i] to each.
 *   - Each thread prints "hello from thread <id>".
 *   - main joins all N threads, then prints "all done".
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g -pthread ex1_hello_threads.c -o ex1
 *   ./ex1       (run it a few times — is the order always the same?)
 *
 * QUESTION
 *   Why pass &ids[i] and not &i (the loop variable)?
 */

#include <stdio.h>
#include <pthread.h>

#define N 4

void *worker( void *arg );

int main( void ) {
    /* TODO */
    return 0;
}

void *worker( void *arg ) {
    /* TODO */
    return NULL;
}
