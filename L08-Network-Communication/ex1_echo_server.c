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
#include <stdlib.h>

#define PORT 2520
#define BUF_SIZE 1024

int main( void ) {
    /* TODO */
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socketfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        return 1;
    }
    if (listen(socketfd, 5) == -1) {
        return 1;
    }

    int clientfd = accept(socketfd, NULL, NULL);
    if (clientfd == -1) {
        return 1;
    }
    int *buf = malloc(BUF_SIZE);
    while (1) {
        ssize_t n = recv( clientfd, buf, BUF_SIZE, 0 );
        if (n > 0) {
            int byteSent = send(clientfd, buf, n, 0);
            if (byteSent == -1) {
                perror("send");
            }
        } else if (n == 0) {
            printf("Client hung up\n");
            break;
        } else {
            perror("recv");
            break;
        }
    }
    close(clientfd);
    close(socketfd);
    free(buf);

    return 0;
}
