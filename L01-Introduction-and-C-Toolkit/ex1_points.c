/*
 * Lecture 1 — Exercise 1: Points on the Heap
 *
 * GOAL
 *   Practice structs, typedef, malloc/free and the -> operator.
 *
 * TASKS
 *   1. Fill in point_t (x and y as doubles).
 *   2. Implement point_new(): malloc a point_t, set x and y, return it
 *      (return NULL if malloc fails).
 *   3. Implement point_print(): print "<label> = (x, y)" with 1 decimal place.
 *   4. In main: make a = (1, 2) and b = (3, 4), print both, free both.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_points.c -o ex1
 *   valgrind --leak-check=full ./ex1      (should report no leaks)
 *
 * EXPECTED OUTPUT
 *   a = (1.0, 2.0)
 *   b = (3.0, 4.0)
 */

#include <stdio.h>
#include <stdlib.h>

/* TODO 1 */
typedef struct {
    double x;
    double y;
} point_t;

point_t *point_new( double x, double y );
void point_print( const char *label, const point_t *p );

int main( int argc, char **argv ) {
    point_t *a = point_new(1, 2);
    point_t *b = point_new(3, 4);

    point_print("a", a);
    point_print("b", b);

    free(a);
    free(b);
    
    return 0;
}

point_t *point_new( double x, double y ) {
    point_t *new = malloc(sizeof(point_t));
    if (new == NULL) {
        return NULL;
    }
    new->x = x;
    new->y = y;

    return new;
}

void point_print( const char *label, const point_t *p ) {
    /* TODO 3 */
    printf("%s = (%.1f, %.1f)\n", label, p->x, p->y);
}
