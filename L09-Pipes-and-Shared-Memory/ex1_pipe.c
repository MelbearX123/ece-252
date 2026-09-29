/*
 * Lecture 9 — Exercise 1: Parent-to-Child Pipe
 *
 * GOAL
 *   Use pipe() + fork() to send a message from parent to child.
 *
 * SPEC
 *   - pipe( fd ), then fork().
 *   - Parent: close the read end, write "Greetings" (strlen + 1 bytes, so
 *     the '\0' is sent too), close the write end, wait(NULL).
 *   - Child: close the write end, read into a buffer, print
 *       "child read: <msg>"
 *     then close the read end.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_pipe.c -o ex1
 *   ./ex1
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main( void ) {
    /* TODO */
    return 0;
}
