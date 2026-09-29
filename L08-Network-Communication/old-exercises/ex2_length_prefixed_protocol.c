/*
 * Lecture 8 — Exercise 2: Framing Messages with a Length Header
 *
 * GOAL
 *   TCP has no message boundaries. Solve the "how do I know they're done
 *   talking?" problem with option 2 from the lecture: send the size first.
 *   Also implement sendall and its mirror image, recvall.
 *
 * PROTOCOL
 *   Each message = 4-byte length L in NETWORK byte order (htonl), followed
 *   by exactly L bytes of payload. A message with L = 0 means "goodbye".
 *
 * TASKS
 *   1. int sendall( int s, const char *buf, int len )
 *        loop over send() until all len bytes are sent; -1 on error.
 *   2. int recvall( int s, char *buf, int len )
 *        loop over recv() until EXACTLY len bytes are received.
 *        Return len on success, 0 if the peer closed before anything
 *        arrived, -1 on error OR if the peer closed mid-message.
 *   3. int send_msg( int s, const char *payload, uint32_t len )
 *        send the header then the payload (both with sendall).
 *   4. char *recv_msg( int s, uint32_t *len_out )
 *        recvall the 4-byte header, ntohl it, malloc len+1, recvall the
 *        payload, null-terminate, return it (caller frees). Return NULL on
 *        error/close. Reject absurd lengths (> 64 MB) — never trust sizes
 *        that come from the network!
 *
 *   5. main: a self-contained test using fork (so you need one terminal):
 *      - Parent: listen on 127.0.0.1:<port> (bind + listen BEFORE fork, so
 *        the child can't connect too early), fork, accept, then loop
 *        recv_msg printing "[server] got <len> bytes: <first 40 chars>"
 *        until L == 0; then close and wait() for the child.
 *      - Child: connect to 127.0.0.1:<port> and send_msg:
 *          "hello"
 *          ""                   <- wait, that's L=0 = goodbye! Pick a
 *                                  different way to handle empty strings,
 *                                  or accept the limitation. Decide.
 *          a 5 MB buffer of 'x' (heap allocated)
 *          "short one after a big one"
 *        then send L=0 and exit.
 *      - Confirm the server sees exactly 4 correct messages, with the right
 *        lengths, and never glues two together or splits one.
 *
 * EXPERIMENT
 *   Replace recvall in recv_msg with a single recv() call. With the 5 MB
 *   message, what goes wrong? This is the bug that bites everyone once.
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex2_length_prefixed_protocol.c -o ex2
 *   ./ex2 2600
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_MSG ( 64u * 1024u * 1024u )

int sendall( int s, const char *buf, int len );
int recvall( int s, char *buf, int len );
int send_msg( int s, const char *payload, uint32_t len );
char *recv_msg( int s, uint32_t *len_out );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int sendall( int s, const char *buf, int len ) { /* TODO */ return -1; }
int recvall( int s, char *buf, int len ) { /* TODO */ return -1; }
int send_msg( int s, const char *payload, uint32_t len ) { /* TODO */ return -1; }
char *recv_msg( int s, uint32_t *len_out ) { /* TODO */ return NULL; }
