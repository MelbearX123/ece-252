/*
 * Lecture 8 — Exercise 1: Echo Server
 *
 * GOAL
 *   Use recv() and send(), and handle recv returning 0 (client left).
 *
 * SPEC
 *   ./ex1
 *   - Set up a listening socket on PORT (same as Lecture 7 ex1).
 *   - Accept ONE client, then loop:
 *       n = recv( client, buf, BUF_SIZE, 0 )
 *       n > 0   -> send the same n bytes back
 *       n == 0  -> print "client hung up" and stop
 *       n < 0   -> perror("recv") and stop
 *   - close both sockets.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_echo_server.c -o ex1
 *   ./ex1
 *   In a second terminal:   nc localhost 2520    (type lines, Ctrl-C to quit)
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 2520
#define BUF_SIZE 1024

int main( void ) {
    /* TODO */
    return 0;
}
