/*
 * Lecture 3 — Exercise 2: Build cp
 *
 * GOAL
 *   The lecture says copy = create + read + write. Build it.
 *
 * SPEC
 *   ./ex2 <src> <dst>
 *   - Open src with "r" and dst with "w". If either fails, perror and return 1.
 *   - Copy one character at a time with fgetc / fputc until EOF.
 *   - Close both files.
 *
 * COMPILE / TEST
 *   gcc -Wall -Wextra -g ex2_mycp.c -o ex2
 *   ./ex2 ex2_mycp.c copy.c
 *   cmp ex2_mycp.c copy.c && echo SAME
 */

#include <stdio.h>

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}
