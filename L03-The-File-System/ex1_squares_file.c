/*
 * Lecture 3 — Exercise 1: Write It, Read It Back
 *
 * GOAL
 *   Practice fopen/fclose, fprintf and fscanf.
 *
 * TASKS
 *   1. write_squares(): open "results.dat" in "w" mode and write the lines
 *        1,1
 *        2,4
 *        ...
 *        10,100
 *   2. read_squares(): open "results.dat" in "r" mode and loop while
 *        fscanf( fp, "%d,%d\n", &i, &sq ) == 2
 *      printing "i: <i>, squared: <sq>".
 *   3. In both functions, check that fopen didn't return NULL.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_squares_file.c -o ex1
 *   ./ex1
 *   cat results.dat
 */

#include <stdio.h>

#define FILENAME "results.dat"

void write_squares( void );
void read_squares( void );

int main( void ) {
    write_squares();
    read_squares();
    return 0;
}

void write_squares( void ) {
    /* TODO */
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        return;
    }
    for (int i = 1; i <= 10; i++) {
        fprintf(fp, "%d, %d\n", i, i*i);
    }
    fclose(fp);
}

void read_squares( void ) {
    /* TODO */
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        return;
    }
    int i;
    int sq;
    while(fscanf( fp, "%d,%d\n", &i, &sq ) == 2) {
        printf("i: %d, squared: %d\n", i, sq);
    }
    fclose(fp);
}
