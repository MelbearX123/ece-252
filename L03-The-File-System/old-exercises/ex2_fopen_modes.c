/*
 * Lecture 3 — Exercise 2: fopen Modes, Predict Then Verify
 *
 * GOAL
 *   Internalize the difference between r, w, a, r+, w+, a+ (and x).
 *
 * SETUP (already written for you)
 *   reset_file() creates "modes.txt" containing exactly:  ABCDEFGHIJ
 *   dump_file() prints the file's current contents between [ ].
 *
 * TASKS
 *   For each experiment below:
 *     1. Write your PREDICTION in the comment before running.
 *     2. Implement the experiment (reset_file() first, then fopen with the
 *        given mode, do the operation, fclose, dump_file()).
 *     3. Run and compare. If you were wrong, write why.
 *
 *   E1  "r"   then fprintf(f, "xyz")               -> file = ?  (return value of fprintf?)
 *   E2  "w"   then fprintf(f, "xyz")               -> file = ?
 *   E3  "a"   then fprintf(f, "xyz")               -> file = ?
 *   E4  "r+"  then fprintf(f, "xyz")               -> file = ?
 *   E5  "w+"  then read one char with fgetc        -> char = ? file = ?
 *   E6  "a+"  then fgetc, then fprintf(f,"xyz")    -> char = ? file = ?
 *   E7  "a"   then fseek(f, 0, SEEK_SET), fprintf(f, "xyz") -> file = ?
 *   E8  "r+"  then fseek(f, 3, SEEK_SET), fprintf(f, "xyz") -> file = ?
 *   E9  "r+"  then fseek(f, -2, SEEK_END), fprintf(f, "xyz") -> file = ?
 *   E10 remove("modes.txt"), then fopen("modes.txt", "r")    -> returns ?
 *   E11 fopen("modes.txt", "wx") when the file EXISTS        -> returns ?
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex2_fopen_modes.c -o ex2
 *   ./ex2
 */

#include <stdio.h>
#include <stdlib.h>

#define FNAME "modes.txt"

static void reset_file( void ) {
    FILE *f = fopen( FNAME, "w" );
    if ( f == NULL ) { perror( "reset" ); exit( 1 ); }
    fprintf( f, "ABCDEFGHIJ" );
    fclose( f );
}

static void dump_file( const char *label ) {
    FILE *f = fopen( FNAME, "r" );
    if ( f == NULL ) { printf( "%-4s file missing\n", label ); return; }
    printf( "%-4s [", label );
    int c;
    while ( ( c = fgetc( f ) ) != EOF ) putchar( c );
    printf( "]\n" );
    fclose( f );
}

int main( void ) {
    /* E1 prediction:  */
    /* TODO */

    /* E2 prediction:  */
    /* TODO */

    /* ... continue through E11 ... */

    (void) reset_file;  /* remove these once you use them */
    (void) dump_file;
    return 0;
}
