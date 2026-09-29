/*
 * Lecture 9 — Exercise 4: Named Pipes (FIFOs) Between Unrelated Programs
 *
 * GOAL
 *   Use mkfifo so two programs started separately can communicate, and
 *   observe FIFO properties: a name on disk, data in memory, persistence
 *   after the creator exits, blocking on open.
 *
 * SPEC — one program, two modes
 *   ./ex4 reader
 *     - mkfifo("chat.fifo", 0600); ignore EEXIST.
 *     - print "waiting for a writer..." then open("chat.fifo", O_RDONLY)
 *       (this BLOCKS until someone opens the other end — observe it!)
 *     - read lines (fdopen + getline) and print "[reader] <line>" until EOF.
 *     - print "writer closed the pipe", close, and exit (do NOT unlink).
 *   ./ex4 writer
 *     - open("chat.fifo", O_WRONLY)  (blocks until a reader exists)
 *     - read lines from stdin, write each to the FIFO, until Ctrl-D; close.
 *   ./ex4 clean
 *     - unlink("chat.fifo")
 *
 * EXPERIMENTS
 *   1. Start the reader first, then the writer. Then the other order.
 *      Which open() blocks in each case?
 *   2. `ls -l chat.fifo` — what's the first character of the permissions?
 *      What size does it show even while data is flowing? (Data is in
 *      memory, not on disk.)
 *   3. Stop both programs. Is chat.fifo still there? (Persistence.)
 *   4. You don't even need your program: in two terminals,
 *        cat chat.fifo          and          echo hello > chat.fifo
 *      "Everything is a file."
 *   5. Start TWO writers and one reader. Do lines interleave cleanly?
 *      (Look up PIPE_BUF: writes <= PIPE_BUF bytes are atomic.)
 *   6. Kill the reader while the writer is still sending. What happens to
 *      the writer? (SIGPIPE — the same as with sockets in Lecture 8.)
 *
 * STRETCH — two-way chat
 *   Use two FIFOs (a_to_b.fifo, b_to_a.fifo) — the lecture notes that a
 *   named pipe carries one direction at a time, so simultaneous two-way
 *   needs two. Fork inside each program: one process reads stdin -> FIFO,
 *   the other reads FIFO -> stdout.
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex4_named_pipe.c -o ex4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "chat.fifo"

int run_reader( void );
int run_writer( void );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int run_reader( void ) { /* TODO */ return 0; }
int run_writer( void ) { /* TODO */ return 0; }
