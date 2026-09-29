/*
 * Lecture 7 — Exercise 3: A Greeting Server (socket, bind, listen, accept)
 *
 * GOAL
 *   Build the server side of a TCP connection with full error checking,
 *   and learn the difference between the listening socket and the
 *   per-client socket.
 *
 * SPEC
 *   ./ex3 <port>        (use something > 1024, e.g. 2520)
 *   - socket(AF_INET, SOCK_STREAM, 0)
 *   - setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one))
 *       (so you can restart the server immediately; try without it later)
 *   - fill a struct sockaddr_in with INADDR_ANY and htons(port); bind
 *   - listen with backlog 5
 *   - print "listening on port <port>"
 *   - LOOP forever:
 *       * accept, capturing the client's address (don't pass NULL)
 *       * print "[server] connection #<n> from <ip>:<port>" — client IP via
 *         inet_ntop, client port via ntohs(client_addr.sin_port)
 *       * send a greeting using write() on the NEW fd (sockets are files!):
 *           "Hello, <ip>:<port>! You are visitor #<n>.\n"
 *       * close the NEW fd (hang up on that client)
 *   - Check EVERY call; on failure perror("<call>") and exit(1).
 *
 * TEST (in another terminal)
 *   nc localhost 2520          (netcat as a client; Ctrl-C to quit)
 *   telnet localhost 2520      (alternative)
 *   Run nc several times. Notice the client PORT changes each time — who
 *   chose it? (Lecture: the client doesn't bind.)
 *   Your ex4 client will also connect to this.
 *
 * QUESTIONS
 *   1. After accept, how many sockets does the server have open? What is
 *      each one for? What happens if you accidentally write() to the
 *      LISTENING socket fd? (Try once — check errno.)
 *   2. While the server is inside accept() with no clients, what process
 *      state is it in (Lecture 4)? Check with ps -o stat.
 *   3. Use `ss -tlnp` (or `netstat -tlnp`) to see your server listening.
 *      What does 0.0.0.0:2520 mean? Relate it to INADDR_ANY.
 *   4. Bind to htonl(INADDR_LOOPBACK) instead of INADDR_ANY. Can you still
 *      connect from localhost? From another machine?
 *
 * STRETCH
 *   Add a SIGINT handler (Lecture 6) that sets a flag; when accept()
 *   returns -1 with errno EINTR and the flag is set, close the listening
 *   socket, print "server shutting down after <n> visitors", and exit 0.
 *   (With signal() on Linux, accept may be auto-restarted; if so, look up
 *   sigaction without SA_RESTART.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex3_greeting_server.c -o ex3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}
