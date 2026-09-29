/*
 * Lecture 5 — Exercise 2: Run a Command (the heart of a shell)
 *
 * GOAL
 *   Combine fork + exec + wait, and decode the status word properly.
 *
 * SPEC
 *   ./ex2 <program> [args...]
 *     e.g.  ./ex2 ls -l /tmp
 *           ./ex2 false
 *           ./ex2 sh -c "exit 42"
 *           ./ex2 nosuchprogram
 *           ./ex2 sleep 30        (then kill it from another terminal)
 *
 *   - Fork. In the child, exec the program with its arguments.
 *       * The lecture uses execlp (argument LIST). Here the number of args
 *         isn't known at compile time, so use execvp(argv[1], &argv[1]) —
 *         the 'v' variant takes an argv-style ARRAY ending in NULL (argv
 *         already ends in NULL!).
 *       * If exec returns, it FAILED: perror("exec") and exit(127)
 *         (127 is what shells use for "command not found").
 *   - In the parent, wait for the child and print ONE of:
 *       "[ex2] child <pid> exited normally with code <n>"
 *       "[ex2] child <pid> was killed by signal <n>"
 *     using WIFEXITED/WEXITSTATUS and WIFSIGNALED/WTERMSIG.
 *   - Also print the RAW status word in hex (%#x) so you can see how the
 *     exit code is packed: try "sh -c 'exit 1'" and "sh -c 'exit 2'".
 *   - main returns the child's exit code (like a shell's $?).
 *
 * QUESTIONS
 *   1. ./ex2 sh -c "exit 300"  -> what code do you get? Why?
 *   2. ./ex2 sleep 30, then from another terminal: kill <child pid>
 *      and kill -9 <child pid>. What signal numbers do you see?
 *   3. Put a printf("after exec\n") right after the exec call. When does
 *      it print?
 *   4. What happens if the child forgets exit(127) after a failed exec and
 *      falls through into the parent's code? (Try it once.)
 *
 * STRETCH — mini shell
 *   Loop: print "mysh> ", read a line with getline, split it on spaces
 *   into an argv array (strtok), fork/exec/wait. Exit on "exit" or EOF.
 *   If the line ends in '&', don't wait (background job!) — and think about
 *   who reaps those children later.
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex2_run_command.c -o ex2
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}
