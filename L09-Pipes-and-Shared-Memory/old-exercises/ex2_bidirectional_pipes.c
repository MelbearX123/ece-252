/*
 * Lecture 9 — Exercise 2: Two Pipes for Two-Way Communication
 *
 * GOAL
 *   Pipes are one-way. Use two (parent->child and child->parent) to build a
 *   request/response "calculator service" in a child process.
 *
 * SPEC
 *   ./ex2
 *   - Create to_child[2] and to_parent[2] BEFORE forking.
 *   - Each process closes the two ends it doesn't use (draw a diagram of
 *     which 4 ends each keeps/closes BEFORE coding).
 *   - Parent reads lines from stdin like:  "add 3 4", "mul 6 7", "div 1 0"
 *     and forwards each as one '\n'-terminated line to the child.
 *   - Child parses the line with sscanf, computes, and replies one line:
 *       "= 7"  /  "= 42"  /  "error: division by zero"  /  "error: bad request"
 *   - Parent prints the reply, then prompts for the next line.
 *   - On EOF (Ctrl-D) the parent closes its write end; the child sees EOF,
 *     prints "[child] no more requests, exiting", and exits 0; the parent
 *     wait()s and prints the child's exit status.
 *   Tip: wrap the fds in FILE* with fdopen() so you can use fprintf/getline,
 *   but then you MUST fflush() after each fprintf — why?
 *
 * DEADLOCK DEMO
 *   Make BOTH processes read first (parent waits for a "ready" line from the
 *   child, child waits for a request from the parent). What happens? This
 *   is a deadlock — each is blocked waiting for the other. Describe it in
 *   Lecture 4 state terms.
 *
 * STRETCH
 *   Also try forgetting the fflush in the child. Where does the reply sit,
 *   and why does the parent block forever?
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex2_bidirectional_pipes.c -o ex2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main( void ) {
    /* TODO */
    return 0;
}
