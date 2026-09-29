/*
 * Lecture 5 — Exercise 1: fork() Basics
 *
 * GOAL
 *   See fork's return values, and that parent and child have SEPARATE memory.
 *
 * TASKS
 *   1. Set int x = 10; then fork().
 *   2. If fork returns < 0: print an error and return 1.
 *   3. Child (fork returned 0): add 5 to x and print
 *        "[child]  pid=<getpid()> x=<x>"
 *   4. Parent (fork returned the child's pid): wait(NULL) for the child,
 *      then print
 *        "[parent] pid=<getpid()> child=<pid> x=<x>"
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_fork_basics.c -o ex1
 *   ./ex1
 *
 * QUESTION
 *   The child changed x. Why does the parent still print 10?
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main( void ) {
    /* TODO */
    return 0;
}
