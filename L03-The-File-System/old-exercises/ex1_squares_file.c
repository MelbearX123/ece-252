/*
 * Lecture 3 — Exercise 1: Write It, Read It Back
 *
 * GOAL
 *   Practice fopen/fclose, fprintf, and fscanf's return value.
 *
 * TASKS
 *   1. write_squares(): open "results.dat" in "w" mode and write the lines
 *        1,1
 *        2,4
 *        ...
 *        n,n*n
 *      using fprintf. Return 0 on success, -1 if fopen fails.
 *   2. read_squares(): open "results.dat" in "r" mode and loop with
 *        fscanf(fp, "%d,%d\n", &i, &isquared) == 2
 *      printing "i: <i>, isquared: <isquared>" for each line. Also VERIFY
 *      that isquared == i*i and count mismatches. Return the number of lines
 *      read, or -1 if fopen fails.
 *   3. main: take n from argv (default 10), write, read, and print
 *        "Read <k> lines, <m> mismatches"
 *   4. Corrupt the file by hand (open it in an editor and change a line to
 *      "7,abc"). Run ONLY the reader (add a flag: ./ex1 read). What does the
 *      loop do when it hits the bad line, and why? What did fscanf return?
 *   5. Change the file so lines have spaces:  "3, 9". Does it still parse?
 *      Why? (Hint: lecture note about %d and leading whitespace.)
 *
 * STRETCH
 *   Rewrite read_squares() using getline() + sscanf() so that a bad line is
 *   reported ("bad line 7: 7,abc") and SKIPPED instead of stopping the loop.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_squares_file.c -o ex1
 *   ./ex1 10
 *   ./ex1 read
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "results.dat"

int write_squares( int n );
int read_squares( void );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int write_squares( int n ) {
    /* TODO */
    return 0;
}

int read_squares( void ) {
    /* TODO */
    return 0;
}
