/*
 * Lecture 13 — Exercise 2: Alice Waits for Bob
 *
 * GOAL
 *   Use a semaphore for SERIALIZATION: make sure one event always happens
 *   before another, without busy-waiting.
 *
 * THE STORY
 *   Alice works the day shift and Bob works the night shift at the power
 *   plant. Alice may NOT go home until Bob has arrived.
 *
 * WHAT'S PROVIDED
 *   alice() and bob() run in two threads. Right now there's no
 *   coordination, so Alice often leaves before Bob arrives.
 *
 * WHAT TO DO
 *   - Make sure "Alice: going home" is ALWAYS printed after
 *     "Bob: arrived", using a semaphore.
 *   - Alice must not busy-wait (no loops checking a variable).
 *   - Set the semaphore up before the threads start, and clean it up
 *     after they've finished.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g -pthread ex2_alice_bob.c -o ex2
 *   ./ex2
 *
 * QUESTIONS
 *   1. What initial value did you give the semaphore? What goes wrong
 *      if you start it at 1?
 *   2. Swap the sleep times so that Bob arrives BEFORE Alice is ready
 *      to leave. Does your solution still work? Why doesn't Bob's
 *      signal get lost?
 *   3. How is this different from Exercise 1? (Hint: in Exercise 1, does
 *      the same thread call both wait and post?)
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define ALICE_WORK_SECONDS 1
#define BOB_COMMUTE_SECONDS 3

void *alice( void *arg ) {
    (void) arg;
    printf( "Alice: working the day shift...\n" );
    sleep( ALICE_WORK_SECONDS );
    printf( "Alice: shift's over, waiting for Bob\n" );
    /* TODO */
    printf( "Alice: going home\n" );
    return NULL;
}

void *bob( void *arg ) {
    (void) arg;
    printf( "Bob: commuting...\n" );
    sleep( BOB_COMMUTE_SECONDS );
    printf( "Bob: arrived\n" );
    /* TODO */
    return NULL;
}

int main( void ) {
    pthread_t a, b;
    /* TODO */
    pthread_create( &a, NULL, alice, NULL );
    pthread_create( &b, NULL, bob, NULL );
    pthread_join( a, NULL );
    pthread_join( b, NULL );
    /* TODO */
    return 0;
}
