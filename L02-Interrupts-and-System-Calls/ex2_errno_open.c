/*
 * Lecture 2 — Exercise 2: When System Calls Fail
 *
 * GOAL
 *   System calls return -1 and set errno on failure. Print a useful message
 *   instead of just "-1".
 *
 * TASKS
 *   Implement try_open(path): call open(path, O_RDONLY) and
 *     - on success print   "OK   <path>"   and close the fd
 *     - on failure print   "FAIL <path>: <message>"   using strerror(errno)
 *   Then call it from main on:
 *     1. "ex2_errno_open.c"   (exists)
 *     2. "nosuchfile.txt"     (doesn't exist)
 *     3. "ex2_errno_open.c/x" (a file used as a directory)
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex2_errno_open.c -o ex2
 *   ./ex2
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

void try_open( const char *path );

int main( void ) {
    /* TODO */
    try_open("ex2_errno_open.c");
    try_open("nosuchfile.txt");
    try_open("ex2_errno_open.c/x");
    return 0;
}

void try_open( const char *path ) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        printf("FAIL %s: %s\n", path, strerror(errno));
        return;
    }
    printf("OK %s\n", path);
    close(fd);
}
