/*
 * Lecture 5 — Exercise 3: Split the Work (Lecture Design Problem, Extended)
 *
 * PART A — implement the lecture's design problem yourself (no peeking).
 *   execute_A() and execute_B() are provided. Their return value is taken
 *   from environment-ish globals set from argv so you can force errors:
 *       ./ex3 A <errA> <errB>
 *   Requirements (same as lecture):
 *     - Child runs execute_B() and returns its result to the parent.
 *     - Parent runs execute_A(), then waits for the child.
 *     - If both succeed print "Completed." and return 0.
 *     - For each failure print "Error <n> Occurred." and return -1.
 *   Test: ./ex3 A 0 0    ./ex3 A 7 0    ./ex3 A 0 9    ./ex3 A 3 4
 *   Verify execute_A and execute_B really run AT THE SAME TIME: they each
 *   sleep(2) — total runtime should be ~2s, not ~4s. (time ./ex3 A 0 0)
 *
 * PART B — N workers, counting primes
 *   ./ex3 B <limit> <nworkers>
 *   Count the primes in [2, limit) by splitting the range into nworkers
 *   equal chunks. Fork one child per chunk; each child counts primes in its
 *   chunk.
 *
 *   PROBLEM: the only thing a child can send back via exit() is 8 bits
 *   (0-255). The count is usually much bigger. For now:
 *     - each child writes its count to a file "count_<i>.txt" (Lecture 3!
 *       file-system IPC — you'll see this again in Lecture 6), then
 *       exits with 0 on success / 1 on failure;
 *     - the parent forks ALL children first, THEN waits for all of them
 *       (why is fork-wait-fork-wait wrong?), checks every exit code,
 *       reads the files, sums, prints, and removes the files.
 *   Time it with nworkers = 1, 2, 4, 8 and limit = 5,000,000.
 *   How does speedup compare to the number of cores (nproc)?
 *
 * COMPILE
 *   gcc -Wall -Wextra -O2 ex3_split_work.c -o ex3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static int err_a = 0;
static int err_b = 0;

int execute_A( void ) { sleep( 2 ); return err_a; }
int execute_B( void ) { sleep( 2 ); return err_b; }

int is_prime( long n ) {
    if ( n < 2 ) return 0;
    for ( long d = 2; d * d <= n; d++ ) {
        if ( n % d == 0 ) return 0;
    }
    return 1;
}

int part_a( void );
int part_b( long limit, int nworkers );

int main( int argc, char **argv ) {
    if ( argc == 4 && argv[1][0] == 'A' ) {
        err_a = atoi( argv[2] );
        err_b = atoi( argv[3] );
        return part_a();
    }
    if ( argc == 4 && argv[1][0] == 'B' ) {
        return part_b( atol( argv[2] ), atoi( argv[3] ) );
    }
    printf( "Usage: %s A <errA> <errB> | B <limit> <nworkers>\n", argv[0] );
    return -1;
}

int part_a( void ) {
    /* TODO */
    return 0;
}

int part_b( long limit, int nworkers ) {
    /* TODO */
    return 0;
}
