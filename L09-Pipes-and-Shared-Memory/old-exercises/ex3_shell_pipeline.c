/*
 * Lecture 9 — Exercise 3: Build a Shell Pipeline  ( ls -l | wc -l )
 *
 * GOAL
 *   See how the shell implements `cat fork.c | less`: pipe + fork + exec,
 *   plus one new call, dup2(), which rewires a program's stdin/stdout.
 *
 * NEW FUNCTION (not in lecture)
 *   int dup2( int oldfd, int newfd );
 *   Makes newfd refer to the same open file/pipe as oldfd (closing newfd
 *   first if needed). So dup2(fd[1], STDOUT_FILENO) means "from now on,
 *   anything written to stdout goes into the pipe". exec keeps open fds, so
 *   the new program inherits the redirection without knowing about it.
 *
 * SPEC
 *   ./ex3                  -> behaves exactly like:  ls -l | wc -l
 *   - pipe(fd)
 *   - fork child 1 ("ls"):  dup2(fd[1], STDOUT_FILENO); close both fd ends;
 *                           execlp("ls", "ls", "-l", NULL); perror; exit(1)
 *   - fork child 2 ("wc"):  dup2(fd[0], STDIN_FILENO); close both fd ends;
 *                           execlp("wc", "wc", "-l", NULL); perror; exit(1)
 *   - parent: close BOTH fd ends, wait for both children, print each
 *     child's exit status to stderr.
 *   Compare output with the real shell pipeline.
 *
 * GENERALIZE
 *   ./ex3 "cat /etc/passwd" "grep bash" "wc -l"
 *   Accept N commands as arguments (split each on spaces into an argv
 *   array; use execvp). You need N-1 pipes. Child i reads from pipe i-1
 *   (if i > 0) and writes to pipe i (if i < N-1). Every process must close
 *   EVERY pipe fd it isn't using.
 *
 * THE CLASSIC BUG
 *   Remove the parent's close(fd[1]). Now `wc` never finishes. Explain
 *   exactly which process still holds a write end and why that prevents
 *   EOF.
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex3_shell_pipeline.c -o ex3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}
