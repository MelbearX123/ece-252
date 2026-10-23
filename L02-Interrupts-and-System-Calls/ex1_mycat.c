/*
 * Lecture 2 — Exercise 1: mycat
 *
 * GOAL
 *   Use the raw system calls open/read/close (not fopen) to print a file.
 *
 * SPEC
 *   ./mycat <file>
 *   - open the file with O_RDONLY. If it returns -1, print
 *     "mycat: cannot open <file>" and return 1.
 *   - Loop: read up to BUF_SIZE - 1 bytes, put a '\0' after them, printf
 *     the buffer. Stop when read returns 0 (end of file).
 *   - close the file and return 0.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_mycat.c -o mycat
 *   ./mycat ex1_mycat.c
 *   ./mycat nosuchfile
 *
 * THINK ABOUT
 *   Why do we read BUF_SIZE - 1 bytes instead of BUF_SIZE?
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 256

int main( int argc, char **argv ) {
    if (argc != 2) {
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        return 1;
    }
    void *buf = malloc(BUF_SIZE * sizeof(int));
    int bytes_read = read(fd, buf, BUF_SIZE - 1);
    if (bytes_read == -1) {
        return 1;
    }

    close(fd);

    return 0;
}
