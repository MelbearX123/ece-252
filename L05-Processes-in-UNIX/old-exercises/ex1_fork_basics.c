/*
 * Lecture 5 — Exercise 1: fork() Basics
 *
 * GOAL
 *   Understand fork's three return cases, that parent and child have
 *   SEPARATE copies of memory, and that output order is up to the scheduler.
 *
 * PART A — who am I?
 *   Fork once. Print:
 *     parent: "[parent] my pid=<getpid()> child pid=<fork result>"
 *     child:  "[child]  my pid=<getpid()> parent pid=<getppid()>"
 *   Handle the fork < 0 case with fprintf(stderr, ...) and return 1.
 *   The parent must wait() for the child before returning.
 *   Run it 5 times. Is the order of the two lines always the same?
 *
 * PART B — separate memory
 *   Before forking, set:  int x = 10;  and  int *heap = malloc(sizeof(int)); *heap = 10;
 *   After fork, the CHILD does x += 5; *heap += 5; and prints both plus &x and heap.
 *   The PARENT waits for the child, then prints x, *heap, &x, heap.
 *   Q: The child and parent print the SAME addresses but DIFFERENT values.
 *      How is that possible? (Hint: virtual memory — each process has its
 *      own address space.)
 *   Q: Who needs to free(heap)? Both? One? (Run under valgrind with
 *      --trace-children=yes.)
 *
 * PART C — counting processes
 *   Run the code in count_forks() with N = 1, 2, 3 (NOT large!).
 *   Predict how many times "hello" prints for each N, then verify:
 *       ./ex1 C 3 | wc -l
 *   Derive the formula. Why is this the core of a fork bomb?
 *   Why does the loop need the wait loop at the end (what would happen
 *   without it)?
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_fork_basics.c -o ex1
 *   ./ex1 A      ./ex1 B      ./ex1 C 3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void part_a( void );
void part_b( void );
void count_forks( int n );

int main( int argc, char **argv ) {
    if ( argc < 2 ) {
        printf( "Usage: %s A | B | C <n>\n", argv[0] );
        return -1;
    }
    switch ( argv[1][0] ) {
        case 'A': part_a(); break;
        case 'B': part_b(); break;
        case 'C': count_forks( argc > 2 ? atoi( argv[2] ) : 1 ); break;
        default:  printf( "Unknown part\n" ); return -1;
    }
    return 0;
}

void part_a( void ) {
    /* TODO */
}

void part_b( void ) {
    /* TODO */
}

void count_forks( int n ) {
    if ( n > 5 ) { printf( "Keep n <= 5, please.\n" ); return; }
    for ( int i = 0; i < n; i++ ) {
        fork();                       /* deliberately unchecked for the puzzle */
    }
    printf( "hello from %d\n", getpid() );
    while ( wait( NULL ) > 0 ) { }    /* reap all of MY children */
}
