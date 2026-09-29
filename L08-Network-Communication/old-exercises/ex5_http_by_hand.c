/*
 * Lecture 8 — Exercise 5: HTTP by Hand over a Raw Socket
 *
 * GOAL
 *   Before using libcurl, do what the lecture says it saves you from: build
 *   an HTTP GET request as a string, send it on a TCP socket, and read the
 *   reply until the server closes the connection (framing option 4).
 *
 * SPEC
 *   ./ex5 <host> [path]          e.g. ./ex5 example.com /
 *   - fd = connect_to(host, "80")    (copy your helper from Lecture 7 ex4)
 *   - Build the request with snprintf:
 *         "GET <path> HTTP/1.0\r\n"
 *         "Host: <host>\r\n"
 *         "User-Agent: ece252-ex5\r\n"
 *         "\r\n"
 *     (The blank line = end of headers. HTTP/1.0 means the server closes
 *      the connection after replying, so "recv returns 0" = done.)
 *   - sendall() the request.
 *   - recv() in a loop into a growing heap buffer (realloc) until recv
 *     returns 0. Handle -1.
 *   - Split the response at the first "\r\n\r\n":
 *       * print the status line (e.g. "HTTP/1.0 200 OK")
 *       * print every header line, indented
 *       * print "body: <n> bytes" and the first 200 chars of the body
 *   - free, close.
 *
 * EXPERIMENTS
 *   1. Forget the final "\r\n" (so no blank line). What happens? Why?
 *   2. Use HTTP/1.1 instead. Does recv still return 0 promptly? Why not?
 *      (Hint: keep-alive; look for the Content-Length header — that's
 *       framing option 2! Implement: if Content-Length is present, stop
 *       after exactly that many body bytes.)
 *   3. Try a site that redirects (e.g. ./ex5 uwaterloo.ca /). What status
 *      code and Location header do you get? Would your program need to
 *      follow it? (libcurl: CURLOPT_FOLLOWLOCATION.)
 *   4. Try port 443 (HTTPS). What comes back and why? (TLS — another reason
 *      to use libcurl.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex5_http_by_hand.c -o ex5
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
int sendall( int s, const char *buf, int len );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int connect_to( const char *host, const char *port ) { /* TODO (from L07 ex4) */ return -1; }
int sendall( int s, const char *buf, int len ) { /* TODO (from ex2) */ return -1; }
