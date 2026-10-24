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
    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo("localhost", "2520", &hints, &res);
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    int status = connect(sockfd, res->ai_addr, res->ai_addrlen);

    char buf[100];
    ssize_t n = recv(sockfd, buf, sizeof(buf) - 1, 0);
    if (n > 0) {
        buf[n] = '\0';
        printf("server says: %s\n", buf);
    }

    close(sockfd);
    freeaddrinfo(res);
    return 0;
}
/*
This is the client side:
We need to setup hints (tells getaddrinfo what type of)
answer we want. We zerod everything except for family
and socktype. It puts the answers in res.

We then use the answers in res to open our socket and
connect to it.

Remember to close socket and freeaddrinfo(res) at end
*/
