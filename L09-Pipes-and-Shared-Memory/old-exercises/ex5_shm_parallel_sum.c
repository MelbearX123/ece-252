/*
 * Lecture 9 — Exercise 5: Shared Memory for Parallel Work
 *
 * GOAL
 *   Use shmget/shmat/shmdt/shmctl so child processes can return LARGE
 *   results (remember Lecture 5: exit codes only carry 8 bits!). Also see a
 *   race condition when two processes write the same location.
 *
 * LAYOUT OF THE SEGMENT
 *   typedef struct {
 *       int n;                 // number of values
 *       int nworkers;
 *       long partial[MAX_WORKERS];   // one slot PER WORKER
 *       long shared_total;     // used only in Part B
 *       int data[];            // n ints (flexible array member)
 *   } shared_t;
 *   size = sizeof(shared_t) + n * sizeof(int)
 *
 * PART A — one slot per worker (safe)
 *   ./ex5 A <n> <nworkers>
 *   - shmget(IPC_PRIVATE, size, IPC_CREAT | 0600); shmat(id, NULL, 0).
 *     Check shmat's return against (void *) -1.
 *   - Parent fills data[i] = i + 1 and sets n, nworkers.
 *   - Fork nworkers children. Child k sums its chunk of data and writes the
 *     sum to partial[k] — its OWN slot. Then shmdt and exit(0).
 *     (Children inherit the attachment — no need to shmat again.)
 *   - Parent waits for all, adds up partial[], compares with n(n+1)/2.
 *   - Parent: shmdt, then shmctl(IPC_RMID).
 *   - Verify with `ipcs -m` that nothing leaked (add a sleep before
 *     IPC_RMID to see it listed, including the "nattch" count changing).
 *
 * PART B — everyone writes one shared total (unsafe!)
 *   ./ex5 B <n> <nworkers>
 *   Same, but each child does, for EACH element:  shm->shared_total += data[i];
 *   (Access it through a  volatile long *  so -O2 doesn't keep the running
 *    total in a register and write it only once at the end.)
 *   Run with n = 10,000,000 and nworkers = 4 several times. Is the answer
 *   right? Is it the same every run? Explain precisely how two "+=" on
 *   different CPUs can lose an update (read-modify-write). This is the
 *   "one process will overwrite another's changes" problem from the
 *   lecture — the coordination tools come later in the course.
 *
 * PART C — ftok and unrelated processes
 *   ./ex5 C write "some text"   and later   ./ex5 C read
 *   Use ftok("ex5_shm_parallel_sum.c", 'S') for the key, a 4096-byte
 *   segment, IPC_CREAT on write. The reader attaches with SHM_RDONLY and
 *   prints the text. Then try writing through the read-only pointer —
 *   what signal kills you? ./ex5 C rm removes the segment.
 *
 * COMPILE
 *   gcc -Wall -Wextra -O2 ex5_shm_parallel_sum.c -o ex5
 */

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define MAX_WORKERS 64

typedef struct {
    int n;
    int nworkers;
    long partial[MAX_WORKERS];
    long shared_total;
    int data[];
} shared_t;

int part_a( int n, int nworkers, int racy );
int part_c( int argc, char **argv );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int part_a( int n, int nworkers, int racy ) { /* TODO (racy != 0 -> Part B behaviour) */ return 0; }
int part_c( int argc, char **argv ) { /* TODO */ return 0; }
