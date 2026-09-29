/*
 * Lecture 1 — Exercise 2: Find the Bugs
 *
 * GOAL
 *   Each function has ONE classic C pitfall from the lecture. For each:
 *   run it, see what goes wrong, fix it, and add a one-line comment.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex2_find_the_bugs.c -o ex2
 *   ./ex2 <1-3>
 *   valgrind ./ex2 <1-3>
 *
 *   Read the compiler warnings — they point at some of the bugs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Bug 1: sum of 1..10 should be 55 */
void bug1( void ) {
    int total;
    for ( int i = 1; i <= 10; i++ ) {
        total += i;
    }
    printf( "total = %d\n", total );
}

/* Bug 2: copy "hello" into a new heap string and print it */
void bug2( void ) {
    const char *src = "hello";
    char *copy = malloc( strlen( src ) );
    strcpy( copy, src );
    printf( "copy = %s\n", copy );
    free( copy );
}

/* Bug 3: store a value, print it, release it */
void bug3( void ) {
    int *value = malloc( sizeof( int ) );
    *value = 252;
    free( value );
    printf( "value = %d\n", *value );
}

int main( int argc, char **argv ) {
    if ( argc != 2 ) {
        printf( "Usage: %s <1-3>\n", argv[0] );
        return -1;
    }
    switch ( atoi( argv[1] ) ) {
        case 1: bug1(); break;
        case 2: bug2(); break;
        case 3: bug3(); break;
        default: printf( "Pick 1-3\n" ); return -1;
    }
    return 0;
}
