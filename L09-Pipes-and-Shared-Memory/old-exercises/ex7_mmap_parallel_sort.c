/*
 * Lecture 9 — Exercise 7: Parallel Sort Through a Memory-Mapped File
 *
 * GOAL
 *   The lecture ends: "this would be great for something like sorting an
 *   array... the sorted array is the same size as the input and we could
 *   share the work." Do exactly that.
 *
 * SPEC
 *   ./ex7 gen <file> <count>      write <count> random ints as raw binary
 *                                 (fwrite an int array) to <file>
 *   ./ex7 sort <file> <nworkers>  sort the file IN PLACE
 *   ./ex7 check <file>            verify ascending order; print first/last 5
 *
 *   sort:
 *   - open(O_RDWR), stat for the size, n = size / sizeof(int).
 *   - int *a = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)
 *   - Split [0, n) into nworkers chunks. Fork one child per chunk; each
 *     child qsort()s ONLY its chunk (directly in the mapping), msyncs,
 *     munmaps, exits.
 *   - Parent waits for all children, then MERGES the sorted chunks. The
 *     merge needs scratch space: malloc a temp array of n ints, merge into
 *     it, memcpy back into the mapping (the file size doesn't change —
 *     that's why mmap fits this problem).
 *   - msync(MS_SYNC), munmap, close.
 *   - Time it (clock_gettime) for nworkers = 1, 2, 4, 8 with
 *     count = 20,000,000. Where does the time go: the parallel qsorts or
 *     the sequential merge?
 *
 * QUESTIONS
 *   1. Why don't the children need any coordination while sorting?
 *      (Hint: which bytes does each one touch?) Contrast with ex5 Part B.
 *   2. What would happen with MAP_PRIVATE instead of MAP_SHARED?
 *   3. Why must the parent wait for ALL children before merging?
 *   4. Could you do this with pipes instead? What would have to be copied,
 *      and how much?
 *
 * COMPILE
 *   gcc -Wall -Wextra -O2 ex7_mmap_parallel_sort.c -o ex7
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

int cmp_int( const void *a, const void *b ) {
    int x = *(const int *) a, y = *(const int *) b;
    return ( x > y ) - ( x < y );
}

int do_gen( const char *path, long count );
int do_sort( const char *path, int nworkers );
int do_check( const char *path );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int do_gen( const char *path, long count ) { /* TODO */ return 0; }
int do_sort( const char *path, int nworkers ) { /* TODO */ return 0; }
int do_check( const char *path ) { /* TODO */ return 0; }
