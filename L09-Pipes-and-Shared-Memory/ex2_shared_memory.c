/*
 * Lecture 9 — Exercise 2: Shared Memory
 *
 * GOAL
 *   Use shmget / shmat / shmdt / shmctl so a child can hand a result back
 *   to its parent.
 *
 * SPEC
 *   - id = shmget( IPC_PRIVATE, sizeof( int ), IPC_CREAT | 0600 )
 *   - int *shared = shmat( id, NULL, 0 ); set *shared = 0.
 *   - Fork.
 *   - Child: set *shared = 252, shmdt( shared ), exit(0).
 *   - Parent: wait(NULL), print "parent sees <*shared>",
 *     then shmdt( shared ) and shmctl( id, IPC_RMID, NULL ).
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex2_shared_memory.c -o ex2
 *   ./ex2
 *
 * QUESTION
 *   In Lecture 5 ex1 the child's change to x was NOT seen by the parent.
 *   Why is this one different?
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main( void ) {
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    int *shared = shmat(shmid, NULL, 0);
    *shared = 0;

    pid_t pid = fork();
    if (pid > 0) {
        wait(NULL);
        printf("Parent sees: %d\n", *shared);
        shmdt(shared);
    } else if (pid == 0) {
        *shared = 252;
        shmdt(shared);
        exit(0);
    } else {
        shmdt(shared);
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
