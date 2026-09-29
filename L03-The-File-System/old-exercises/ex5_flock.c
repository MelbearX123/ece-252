/*
 * Lecture 3 — Exercise 5: File Locks Across Processes
 *
 * GOAL
 *   See flock() exclusive and shared locks interact between two running
 *   programs, and learn that UNIX locks are ADVISORY.
 *
 * SPEC
 *   ./ex5 <ex|sh> <seconds> [nb]
 *   - fopen("shared.log", "a+"), get the fd with fileno().
 *   - Print "[pid] requesting <EX|SH> lock..." and call flock() with
 *     LOCK_EX or LOCK_SH (OR in LOCK_NB if "nb" was given).
 *   - If flock fails (only possible with nb), print the errno name
 *     (EWOULDBLOCK) and exit 1.
 *   - Print "[pid] GOT lock", then once per second for <seconds> seconds
 *     append a line "[pid] tick i" to the file with fprintf + fflush.
 *   - flock(fd, LOCK_UN), print "[pid] released", fclose.
 *   (getpid() from <unistd.h> gives your process id — covered in L05.)
 *
 * EXPERIMENTS (use two terminals in the same directory)
 *   1. T1: ./ex5 ex 10    T2: ./ex5 ex 3
 *      -> When does T2 print "GOT lock"? Look at shared.log: are ticks
 *         interleaved?
 *   2. T1: ./ex5 sh 10    T2: ./ex5 sh 3      -> do they both get it?
 *   3. T1: ./ex5 sh 10    T2: ./ex5 ex 3      -> ?
 *   4. T1: ./ex5 ex 10    T2: ./ex5 ex 3 nb   -> ?
 *   5. T1: ./ex5 ex 10    T2: echo hi >> shared.log
 *      -> Did the lock stop the echo? What does "advisory" mean?
 *   6. T1: ./ex5 ex 30    T2: rm shared.log ; ls
 *      -> Does T1 crash? Does it keep writing successfully? Where do its
 *         writes go? (Lecture: UNIX deletion vs. an open file.)
 *   Write the observed results in the comment block at the bottom.
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex5_flock.c -o ex5
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/file.h>

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

/*
 * OBSERVATIONS
 *   1.
 *   2.
 *   3.
 *   4.
 *   5.
 *   6.
 */
