/*
 * Lecture 7 — Exercise 1: Greeting Server
 *
 * GOAL
 *   The server side: socket -> bind -> listen -> accept.
 *
 * SPEC
 *   ./ex1
 *   - Create a TCP socket: socket( AF_INET, SOCK_STREAM, 0 )
 *   - Fill in a struct sockaddr_in: AF_INET, port htons( PORT ),
 *     address INADDR_ANY. bind() to it, then listen( sock, 5 ).
 *   - Loop forever: accept() a client, send it "Hello from the server!\n",
 *     close() the client socket.
 *   - Check each call for -1 and perror on failure.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex1_greeting_server.c -o ex1
 *   ./ex1
 *   In a second terminal:   nc localhost 2520
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 2520

int main( void ) {
    /* TODO */
    return 0;
}
