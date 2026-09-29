/*
 * Lecture 7 — Exercise 2: Client
 *
 * GOAL
 *   The client side: getaddrinfo -> socket -> connect.
 *
 * SPEC
 *   ./ex2
 *   - getaddrinfo( "localhost", "2520", &hints, &res ) with
 *     hints.ai_family = AF_INET and hints.ai_socktype = SOCK_STREAM.
 *   - socket() and connect() using res->ai_addr / res->ai_addrlen.
 *   - recv() up to 99 bytes, add a '\0', print what the server sent.
 *   - close() the socket and freeaddrinfo( res ).
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex2_client.c -o ex2
 *   Start ex1 in one terminal, then run ./ex2 in another.
 *
 * TRY
 *   Run ./ex2 with the server stopped. What error do you get?
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main( void ) {
    /* TODO */
    return 0;
}
