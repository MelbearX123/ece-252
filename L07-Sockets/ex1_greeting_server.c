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
    int socketfd = socket(AF_INET, SOCK_STREAM,0);
    if (socketfd == -1) {
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        return 1;
    }
    if (listen(socketfd, 5) == -1) {
        return 1;
    }

    while (1) {
        int clientfd = accept(socketfd, NULL, NULL);
        if (clientfd == -1) {
            return 1;
        }
        const char *greeting = "Hello from the server!\n";
        send(clientfd, greeting, strlen(greeting), 0);

        close(clientfd);
    }

    return 0;
}
/*
This is server side:
We first need to make the socket and setup the address
information. We can then bind it, and then listen to it.

Infinite loop to keep the socket open. We accept
a clientfd in there. Accept is blocking, so the code
past it only runs once there's an accept event.
We then send the greeting from the client to the server.
After this, close the server.
*/
