/*
 * Lecture 1 — Exercise 2: Command-Line Stats on a Heap Array
 *
 * GOAL
 *   Practice argc/argv, atoi, heap arrays, memset, passing arrays WITH a
 *   length, the "first parameter is modified + int result code" convention,
 *   and returning a non-zero code from main on error.
 *
 * PROGRAM SPEC
 *   Usage: ./ex2 <n> <v1> <v2> ... <vn>
 *     e.g. ./ex2 5 4 8 15 16 23
 *
 *   - If argc is wrong (fewer than 2, or not exactly n+2 args) print
 *       Usage: ./ex2 <n> <v1> ... <vn>
 *     and return -1 from main.
 *   - Allocate an int array of n elements on the heap (malloc + sizeof).
 *   - Zero it with memset (NOT a loop).
 *   - Fill it from argv using atoi.
 *   - Call compute_stats() to fill a stats_t struct.
 *   - Print the results exactly like:
 *       n=5 min=4 max=23 sum=66 mean=13.20
 *   - Free the array.
 *
 * compute_stats CONTRACT (lecture convention)
 *   int compute_stats( stats_t *out, const int *data, int length );
 *   - out is the thing being filled in (first parameter).
 *   - Returns 0 on success.
 *   - Returns STATS_ERR_NULL if out or data is NULL.
 *   - Returns STATS_ERR_EMPTY if length <= 0.
 *   main should print "Error <code>" and return -1 if it fails.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex2_dynamic_array.c -o ex2
 *   ./ex2 5 4 8 15 16 23
 *   ./ex2 0                  -> should report the STATS_ERR_EMPTY error code
 *   ./ex2 3 1 2              -> usage message
 *   valgrind ./ex2 5 4 8 15 16 23
 *
 * THINK ABOUT
 *   - What does atoi("hello") return? What about atoi("12abc")? Try it.
 *     (Stretch: use strtol and check the end pointer to reject bad input.)
 *   - Why does compute_stats need the `length` parameter at all?
 *   - What happens if you print mean with %d instead of %.2f? Try it once and
 *     read the compiler warning.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STATS_ERR_NULL  1
#define STATS_ERR_EMPTY 2

typedef struct {
    int min;
    int max;
    long sum;
    double mean;
} stats_t;

int compute_stats( stats_t *out, const int *data, int length );

int main( int argc, char **argv ) {
    /* TODO: validate argc, parse n, allocate, memset, fill, compute, print, free */
    return 0;
}

int compute_stats( stats_t *out, const int *data, int length ) {
    /* TODO */
    return 0;
}
