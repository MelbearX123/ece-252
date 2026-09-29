/*
 * Lecture 1 — Exercise 1: Points on the Heap
 *
 * GOAL
 *   Practice structs, typedef, heap allocation with malloc/sizeof, the arrow
 *   operator, pass-by-pointer, function prototypes, and printf formatting.
 *
 * TASKS
 *   1. Complete the typedef for point_t (x, y, z as doubles).
 *   2. Implement point_new(): malloc a point_t, initialize its fields from the
 *      arguments, and return the pointer. Return NULL if malloc fails.
 *   3. Implement point_add(): return a NEW heap-allocated point that is the sum
 *      of the two inputs. (Who is responsible for freeing it? The caller.)
 *   4. Implement point_scale(): modify the point in place (first parameter is
 *      the one modified — lecture convention). Use -> syntax.
 *   5. Implement point_print(): print as "(x, y, z)" with 1 decimal place.
 *   6. In main, build the points described in the comments, print them, and
 *      free EVERY allocation. Run under valgrind and confirm
 *      "All heap blocks were freed -- no leaks are possible".
 *   7. Notice that the helper functions are defined BELOW main. Remove the
 *      prototypes block temporarily, recompile, and read the error/warning.
 *      Put them back.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_points.c -o ex1
 *   ./ex1
 *   valgrind --leak-check=full ./ex1
 *
 * EXPECTED OUTPUT
 *   a = (1.0, 2.0, 3.0)
 *   b = (-0.5, 4.5, 10.0)
 *   a + b = (0.5, 6.5, 13.0)
 *   2 * (a + b) = (1.0, 13.0, 26.0)
 *
 * STRETCH
 *   - Add a double point_distance(const point_t *p, const point_t *q) using
 *     sqrt from <math.h> (link with -lm).
 *   - Allocate an ARRAY of 5 point_t on the heap with ONE malloc call, fill it
 *     in a loop, print it, then free it with ONE free call.
 */

#include <stdio.h>
#include <stdlib.h>

/* TODO 1: define point_t */
typedef struct {
    /* fields here */
} point_t;

/* Prototypes — the definitions are below main. */
point_t *point_new( double x, double y, double z );
point_t *point_add( const point_t *p, const point_t *q );
void point_scale( point_t *p, double factor );
void point_print( const char *label, const point_t *p );

int main( int argc, char **argv ) {
    /* TODO 6:
     *   a = (1.0, 2.0, 3.0)
     *   b = (-0.5, 4.5, 10.0)
     *   sum = a + b
     *   print a, b, sum
     *   scale sum by 2 and print it again with label "2 * (a + b)"
     *   free everything
     */
    return 0;
}

point_t *point_new( double x, double y, double z ) {
    /* TODO 2 */
    return NULL;
}

point_t *point_add( const point_t *p, const point_t *q ) {
    /* TODO 3 */
    return NULL;
}

void point_scale( point_t *p, double factor ) {
    /* TODO 4 */
}

void point_print( const char *label, const point_t *p ) {
    /* TODO 5 */
}
