/*
 * Lecture 13 — Exercise 1: Protect the Linked List
 *
 * GOAL
 *   See the lecture's linked list end up in an inconsistent state when
 *   several threads push at once, then fix it with a semaphore.
 *
 * WHAT'S PROVIDED
 *   The list and push_front() exactly as in the lecture (no protection),
 *   plus a main() that starts NUM_THREADS threads, each pushing
 *   PUSHES_PER_THREAD items, and then checks the list:
 *     - does size match the number of nodes actually in the list?
 *     - is that the number of items that were pushed?
 *     - does tail really point at the last node?
 *
 * STEPS
 *   1. Compile and run it as-is, several times. Which checks fail? Are
 *      the results the same every run?
 *   2. Fix it: give the list a semaphore and use it so that only one
 *      thread at a time modifies the list. Remember to set it up and to
 *      clean it up. Keep the critical section as small as it can be
 *      while still covering every access to the shared list.
 *   3. Run it several times again. Every check should pass every time.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g -pthread ex1_list_semaphore.c -o ex1
 *   ./ex1
 *
 * QUESTIONS
 *   1. In a run where the counts didn't match, what happened to the
 *      missing nodes? (Think about two threads both reading the same
 *      old head.)
 *   2. Why shouldn't the malloc call be inside the critical section?
 *   3. What initial value did you give the semaphore, and why that one?
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 4
#define PUSHES_PER_THREAD 100000

typedef struct single_node {
    void *element;
    struct single_node *next;
} single_node_t;

typedef struct single_list {
    single_node_t *head;
    single_node_t *tail;
    int size;
    /* TODO */
} single_list_t;

single_list_t list;

void single_list_init( single_list_t *list ) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    /* TODO */
}

bool push_front( single_list_t *list, void *obj ) {
    single_node_t *tmp = malloc( sizeof( single_node_t ) );
    if ( tmp == NULL ) {
        return false;
    }
    tmp->element = obj;
    tmp->next = list->head;
    list->head = tmp;
    if ( list->size == 0 ) {
        list->tail = tmp;
    }
    ++(list->size);
    return true;
}

/* ---- Test harness below: you shouldn't need to change it (except the
        TODO for cleaning up). ---- */

void *pusher( void *arg ) {
    (void) arg;
    for ( int i = 0; i < PUSHES_PER_THREAD; i++ ) {
        push_front( &list, NULL );
    }
    return NULL;
}

int main( void ) {
    single_list_init( &list );

    pthread_t threads[NUM_THREADS];
    for ( int i = 0; i < NUM_THREADS; i++ ) {
        pthread_create( &threads[i], NULL, pusher, NULL );
    }
    for ( int i = 0; i < NUM_THREADS; i++ ) {
        pthread_join( threads[i], NULL );
    }

    /* Walk the list: count the nodes and find the real last node. */
    int nodes = 0;
    single_node_t *last = NULL;
    for ( single_node_t *n = list.head; n != NULL; n = n->next ) {
        nodes++;
        last = n;
    }

    int expected = NUM_THREADS * PUSHES_PER_THREAD;
    printf( "pushed:            %d\n", expected );
    printf( "size field says:   %d\n", list.size );
    printf( "nodes in the list: %d\n", nodes );
    printf( "size matches nodes:   %s\n", list.size == nodes ? "yes" : "NO" );
    printf( "nothing lost:         %s\n", nodes == expected ? "yes" : "NO" );
    printf( "tail is last node:    %s\n", list.tail == last ? "yes" : "NO" );

    /* Free the nodes that are still reachable. */
    single_node_t *n = list.head;
    while ( n != NULL ) {
        single_node_t *next = n->next;
        free( n );
        n = next;
    }
    /* TODO */
    return 0;
}
