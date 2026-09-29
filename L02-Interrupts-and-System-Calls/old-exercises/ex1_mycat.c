/*
 * Lecture 2 — Exercise 1: mycat
 *
 * GOAL
 *   Use the raw system calls open/read/close (not fopen!) to print files,
 *   and handle every return value properly.
 *
 * SPEC
 *   Usage: ./mycat <file1> [file2 ...]
 *   - Print the contents of each file in order to standard output.
 *   - If a file can't be opened, print "mycat: cannot open <name>" to stderr,
 *     SKIP it, continue with the others, and make main return 1 at the end.
 *   - Otherwise return 0.
 *
 * REQUIREMENTS
 *   - Use a fixed buffer of BUF_SIZE bytes allocated with malloc.
 *   - Part A: print with printf("%s") like the lecture. This REQUIRES you to
 *     null-terminate what you read. Do it by writing buffer[bytes_read] = '\0'
 *     (not memset) — make sure you read at most BUF_SIZE - 1 bytes.
 *   - Handle read returning -1 (print an error and stop reading that file).
 *   - Part B: switch to write(STDOUT_FILENO, buffer, bytes_read) instead of
 *     printf. Why does this version NOT need a null terminator? Why does it
 *     work correctly for binary files (e.g. ./mycat ./mycat | cmp - ./mycat)
 *     while the printf version doesn't?
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_mycat.c -o mycat
 *   ./mycat ex1_mycat.c
 *   ./mycat ex1_mycat.c nosuchfile README.md ; echo "exit code: $?"
 *   diff <(./mycat README.md) README.md     (no output = identical)
 *
 * THINK ABOUT
 *   - Set BUF_SIZE to 4. Does the output still come out right? Why?
 *   - What does `read` return on the call AFTER the last chunk?
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 256

/* Prints the whole file behind fd. Returns 0 on success, -1 on read error. */
int copy_fd_to_stdout( int fd, char *buffer );

int main( int argc, char **argv ) {
    /* TODO: usage check, allocate buffer, loop over argv[1..], open,
             copy_fd_to_stdout, close, track failures, free, return code */
    return 0;
}

int copy_fd_to_stdout( int fd, char *buffer ) {
    /* TODO */
    return 0;
}
