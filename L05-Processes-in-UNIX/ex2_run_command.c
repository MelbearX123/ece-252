/*
 * Lecture 5 — Exercise 2: Run a Command
 *
 * GOAL
 *   Combine fork + exec + wait — the core of every shell.
 *
 * SPEC
 *   ./ex2
 *   - Fork.
 *   - Child: execlp( "ls", "ls", "-l", NULL ). If exec returns, it failed:
 *     print an error and exit(1).
 *   - Parent: wait(&status) for the child, then print
 *       "child exited with code <WEXITSTATUS(status)>"
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex2_run_command.c -o ex2
 *   ./ex2
 *
 * TRY
 *   Change "ls" to "nosuchprogram". What exit code does the parent see?
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main( void ) {
    pid_t pid = fork();
    int status;
    if (pid > 0) {
        wait(&status);
        printf("child exited with code %d\n", status);
    } else if (pid == 0) {
        execlp( "ls", "ls", "-l", NULL );
    } else {
        return 1;
    }
    return 0;
}
