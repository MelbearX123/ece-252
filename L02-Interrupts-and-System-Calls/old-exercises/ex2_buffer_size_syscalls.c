/*
 * Lecture 2 — Exercise 2: The Cost of Crossing into the Kernel
 *
 * GOAL
 *   Every read() is a trap into the kernel: mode switch, handler lookup,
 *   mode switch back. See for yourself how the NUMBER of system calls affects
 *   run time by reading the same file with different buffer sizes.
 *
 * SPEC
 *   Usage: ./ex2 <file> <buffer_size>
 *   - Read the entire file with read() using a heap buffer of <buffer_size>.
 *   - Count: total bytes read, number of read() calls made (including the
 *     final one that returns 0).
 *   - Time the loop with clock_gettime(CLOCK_MONOTONIC, ...).
 *   - Print:  bytes=<n> calls=<n> time_ms=<x.xxx>
 *
 * EXPERIMENT
 *   1. Make a large test file (~50 MB):
 *        head -c 50000000 /dev/urandom > big.bin
 *   2. Run with buffer sizes 1, 16, 256, 4096, 65536, 1048576.
 *      (Size 1 will be slow — that's the point. Be patient.)
 *   3. Record the results in a table in the comment block at the bottom.
 *   4. Also run:  time ./ex2 big.bin 1   and   time ./ex2 big.bin 65536
 *      Compare "user" vs "sys" time. Which one dominates for size 1? Why?
 *
 * QUESTIONS (answer in the comment block at the bottom)
 *   a. Roughly how does calls relate to file size / buffer size?
 *   b. Why does time stop improving past a certain buffer size?
 *   c. Relate this to the 8 steps of a system call from the lecture: which
 *      steps are paid PER CALL regardless of how many bytes are moved?
 *
 * COMPILE
 *   gcc -Wall -Wextra -O2 ex2_buffer_size_syscalls.c -o ex2
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

/*
 * RESULTS
 *   buffer_size | calls | time_ms
 *   ------------+-------+--------
 *             1 |       |
 *            16 |       |
 *           256 |       |
 *          4096 |       |
 *         65536 |       |
 *       1048576 |       |
 *
 * ANSWERS
 *   a.
 *   b.
 *   c.
 */
