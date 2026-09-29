/*
 * Lecture 7 — Exercise 4: The connect_to() Helper and a Client
 *
 * GOAL
 *   Write the reusable helper the lecture suggests, hiding all client-side
 *   boilerplate, and use it to talk to your Exercise 3 server.
 *
 * TASK 1 — implement
 *   int connect_to( const char *host, const char *port );
 *   - getaddrinfo(host, port, hints{AF_INET, SOCK_STREAM}, &res)
 *     (on failure: fprintf stderr with gai_strerror, return -1)
 *   - For EACH result in the list (not just the first!):
 *       socket(res->ai_family, res->ai_socktype, res->ai_protocol)
 *       connect(fd, res->ai_addr, res->ai_addrlen)
 *       on success: break; on failure: remember errno, close(fd), try next
 *   - freeaddrinfo(res) in ALL paths.
 *   - Return the connected fd, or -1 (with errno preserved from the last
 *     failed connect so the caller can inspect it).
 *
 * TASK 2 — client
 *   ./ex4 <host> <port>
 *   - fd = connect_to(host, port). If -1, print a specific message:
 *       ECONNREFUSED -> "nobody is listening on <host>:<port>"
 *       ETIMEDOUT    -> "connection timed out"
 *       otherwise    -> perror
 *   - read() from fd in a loop into a buffer until read returns 0 (the
 *     server hung up), printing whatever arrives.
 *   - close(fd).
 *
 * TEST
 *   ./ex3 2520 &            (your server)
 *   ./ex4 localhost 2520
 *   ./ex4 127.0.0.1 2520
 *   ./ex4 localhost 2521    (nothing there -> ECONNREFUSED)
 *   ./ex4 10.255.255.1 80   (unroutable -> hangs, then ETIMEDOUT; takes a while)
 *
 * TASK 3 — talk to a real web server by hand
 *   ./ex4 example.com 80    -> it connects... and then nothing happens.
 *   Why? (Who is supposed to talk first in HTTP?) We fix this in Lecture 8
 *   by SENDING a request. For now, try:  printf 'GET / HTTP/1.0\r\nHost:
 *   example.com\r\n\r\n' | nc example.com 80   to see what the request
 *   looks like.
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex4_client_connect_to.c -o ex4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int connect_to( const char *host, const char *port );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int connect_to( const char *host, const char *port ) {
    /* TODO */
    return -1;
}
