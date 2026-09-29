/*
 * Lecture 9 — Exercise 1: Pipes and Message Boundaries
 *
 * PART A — reproduce and fix the lecture bug
 *   The function lecture_version() below is the lecture's code. Run
 *   ./ex1 A several times (try piping through `cat -v` to see garbage
 *   characters: ./ex1 A | cat -v). Then write fixed_version() that:
 *     - memsets read_msg to 0 before reading
 *     - reads at most BUFFER_SIZE - 1 bytes
 *     - checks the return values of pipe, fork, write, read
 *     - has the parent wait() for the child
 *
 * PART B — several messages through one pipe
 *   ./ex1 B
 *   The parent sends these 4 messages back-to-back with NO delay:
 *      "alpha", "bravo", "charlie delta echo", "foxtrot"
 *   The child must print each on its own line:  [child] msg 0: alpha ...
 *
 *   B1: first try the naive way — one write() per message, one read() per
 *       message in the child with a 64-byte buffer. What does the child
 *       actually get? (Lecture: the pipe is a byte queue; boundaries are
 *       NOT preserved.)
 *   B2: fix it with a TERMINATOR: send each message followed by '\n'; the
 *       child wraps fd[0] in a FILE* with fdopen(fd[0], "r") and uses
 *       getline() (Lecture 3!) to read one message at a time.
 *   B3: fix it with a LENGTH PREFIX: write a uint32_t length, then the
 *       bytes. The child reads exactly 4 bytes, then exactly len bytes
 *       (loop — read can return fewer!). Stop when read returns 0 (EOF).
 *
 * PART C — EOF depends on closing
 *   In B3, delete the child's close(fd[WRITE_END]) line. Run it. The child
 *   hangs after the last message — why? Who still holds a write end?
 *   (Use `ls -l /proc/<childpid>/fd` in another terminal to see.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex1_pipe_messages.c -o ex1
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int lecture_version( void ) {
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    pid_t pid;

    if ( pipe( fd ) == -1 ) {
        fprintf( stderr, "Pipe failed" );
        return 1;
    }
    pid = fork();
    if ( pid < 0 ) {
        fprintf( stderr, "Fork Failed" );
        return 1;
    }
    if ( pid > 0 ) {
        close( fd[READ_END] );
        write( fd[WRITE_END], write_msg, strlen( write_msg ) );
        close( fd[WRITE_END] );
    } else {
        close( fd[WRITE_END] );
        read( fd[READ_END], read_msg, BUFFER_SIZE );
        printf( "read %s", read_msg );
        close( fd[READ_END] );
    }
    return 0;
}

int fixed_version( void );
int multi_naive( void );
int multi_terminator( void );
int multi_length_prefix( void );

int main( int argc, char **argv ) {
    if ( argc < 2 ) {
        printf( "Usage: %s A | Afix | B1 | B2 | B3\n", argv[0] );
        return -1;
    }
    if ( strcmp( argv[1], "A" ) == 0 )    return lecture_version();
    if ( strcmp( argv[1], "Afix" ) == 0 ) return fixed_version();
    if ( strcmp( argv[1], "B1" ) == 0 )   return multi_naive();
    if ( strcmp( argv[1], "B2" ) == 0 )   return multi_terminator();
    if ( strcmp( argv[1], "B3" ) == 0 )   return multi_length_prefix();
    return -1;
}

int fixed_version( void ) { /* TODO */ return 0; }
int multi_naive( void ) { /* TODO */ return 0; }
int multi_terminator( void ) { /* TODO */ return 0; }
int multi_length_prefix( void ) { /* TODO */ return 0; }
