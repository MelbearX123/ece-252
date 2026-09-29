/*
 * Lecture 8 — Exercise 1: TCP Echo Server
 *
 * GOAL
 *   Use recv() and send() on the per-client socket, correctly handling
 *   partial data, the "peer closed" case (recv == 0), and errors (-1).
 *
 * SPEC
 *   ./ex1 <port>
 *   - Set up a listening socket (reuse your Lecture 7 ex3 code; put it in a
 *     helper: int listen_on(uint16_t port)).
 *   - Loop: accept a client, print "client connected: ip:port".
 *       Inner loop:
 *         n = recv(client, buf, BUF_SIZE, 0)
 *         n > 0  -> echo back EXACTLY those n bytes (use sendall from the
 *                   lecture — implement it below). Also print
 *                   "[recv <n> bytes]" to the server console.
 *         n == 0 -> print "client hung up", break
 *         n < 0  -> perror("recv"), break
 *       close(client). Keep serving the next client.
 *   - Special command: if the client sends a line that is exactly "quit\n",
 *     reply "bye\n" and close that client.
 *     (Careful: a single recv might contain "hel" or "lo\nquit\n" — TCP is
 *      a byte stream, not messages! For this exercise, accept that your
 *      quit detection might be imperfect, and note WHEN it would fail.)
 *
 * TEST
 *   nc localhost 2520       type lines, see them echoed
 *   Set BUF_SIZE to 4 and type a long line. How many "[recv ...]" prints
 *   do you get for one line? What does that prove about recv?
 *   yes | head -c 10000000 | nc -q1 localhost 2520 | wc -c
 *       -> should print 10000000. If your echo drops bytes, you have a
 *          partial-send bug.
 *
 * QUESTIONS
 *   1. Only one client is served at a time. Connect two nc's — what does
 *      the second one experience? (Lecture 10 threads will fix this.)
 *   2. What happens to the server if the client disconnects while the
 *      server is in send()? (Look up SIGPIPE; try signal(SIGPIPE, SIG_IGN)
 *      and check for errno EPIPE instead.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex1_echo_server.c -o ex1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

int listen_on( uint16_t port );
int sendall( int socket, char *buf, int *len );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int listen_on( uint16_t port ) { /* TODO */ return -1; }
int sendall( int socket, char *buf, int *len ) { /* TODO */ return -1; }
