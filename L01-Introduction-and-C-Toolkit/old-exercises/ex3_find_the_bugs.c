/*
 * Lecture 1 — Exercise 3: Find the Bugs
 *
 * GOAL
 *   Every function below contains at least one of the classic pitfalls from
 *   Lecture 1. For each one:
 *     (a) PREDICT what goes wrong before running it,
 *     (b) run it (normally AND under valgrind) and see what actually happens,
 *     (c) fix it, and write a one-line comment explaining the bug.
 *
 * Pitfalls hidden here (one or more per function):
 *   uninitialized variable, missing null terminator / no room for it,
 *   #define precedence, wrong printf specifier, too few printf arguments,
 *   use-after-free, double free, memset misuse, off-by-one array access,
 *   memory leak, freeing stack memory.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex3_find_the_bugs.c -o ex3
 *   ./ex3 <1-8>            (runs one bug at a time so a crash doesn't hide others)
 *   valgrind ./ex3 <1-8>
 *
 *   Pay attention to compiler WARNINGS — several bugs are flagged by -Wall.
 *   Then try compiling with -fsanitize=address and running again.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AREA 10 + 5            /* bug 1 lives here */

/* Bug 1: should print 30 (i.e. (10 + 5) * 2) */
void bug1( void ) {
    int doubled = AREA * 2;
    printf( "doubled area = %d\n", doubled );
}

/* Bug 2: sum of 1..10 should be 55 */
void bug2( void ) {
    int total;
    for ( int i = 1; i <= 10; i++ ) {
        total += i;
    }
    printf( "total = %d\n", total );
}

/* Bug 3: copy "hello" into a new heap string and print it */
void bug3( void ) {
    const char *src = "hello";
    char *copy = malloc( strlen( src ) );
    for ( size_t i = 0; i < strlen( src ); i++ ) {
        copy[i] = src[i];
    }
    printf( "copy = %s\n", copy );
    free( copy );
}

/* Bug 4: print the average of three values */
void bug4( void ) {
    double avg = ( 3 + 4 + 6 ) / 3.0;
    int count = 3;
    printf( "avg = %d over %d values, max = %d\n", avg, count );
}

/* Bug 5: initialize an array to all ones and print it */
void bug5( void ) {
    int *ones = malloc( 5 * sizeof( int ) );
    memset( ones, 1, 5 );
    for ( int i = 0; i <= 5; i++ ) {
        printf( "%d ", ones[i] );
    }
    printf( "\n" );
    free( ones );
}

/* Bug 6: store a value, release it, print it */
void bug6( void ) {
    int *value = malloc( sizeof( int ) );
    *value = 252;
    free( value );
    printf( "value = %d\n", *value );
    free( value );
}

/* Bug 7: build 3 strings in a loop */
void bug7( void ) {
    for ( int i = 0; i < 3; i++ ) {
        char *line = malloc( 32 );
        sprintf( line, "line %d", i );
        printf( "%s\n", line );
    }
}

/* Bug 8: "clean up" a local counter */
void bug8( void ) {
    int counter = 0;
    int *p = &counter;
    *p = 42;
    printf( "counter = %d\n", counter );
    free( p );
}

int main( int argc, char **argv ) {
    if ( argc != 2 ) {
        printf( "Usage: %s <1-8>\n", argv[0] );
        return -1;
    }
    switch ( atoi( argv[1] ) ) {
        case 1: bug1(); break;
        case 2: bug2(); break;
        case 3: bug3(); break;
        case 4: bug4(); break;
        case 5: bug5(); break;
        case 6: bug6(); break;
        case 7: bug7(); break;
        case 8: bug8(); break;
        default: printf( "Pick 1-8\n" ); return -1;
    }
    return 0;
}
