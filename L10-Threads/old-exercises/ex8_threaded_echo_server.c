/*
 * Lecture 10 — Exercise 8: A Thread-per-Client Echo Server
 *
 * GOAL
 *   Tie together Lectures 7, 8 and 10. Your Lecture 8 echo server could
 *   only serve one client at a time. Give each accepted client its own
 *   thread so the main thread can go straight back to accept().
 *
 * SPEC
 *   ./ex8 <port>
 *   - Listening socket as before (reuse listen_on() and sendall()).
 *   - Loop:
 *       int client = accept(...);
 *       client_args_t *a = malloc(sizeof *a);  a->fd = client; a->id = ++n;
 *       copy the client's ip:port string into a as well
 *       pthread_create(&t, NULL, handle_client, a);
 *       pthread_detach(t);     // nobody will join; resources auto-freed
 *   - handle_client(arg):
 *       * copy what you need out of arg, then free(arg)   (who owns it?)
 *       * echo loop with recv/sendall until recv returns 0 or -1
 *       * print "[client <id>] connected / disconnected (<bytes> bytes echoed)"
 *       * close(fd); return NULL
 *
 * TEST
 *   Open 3 terminals running `nc localhost 2520` at the same time and type
 *   in each — all should be echoed concurrently (unlike Lecture 8 ex1).
 *   Stress it:
 *     for i in $(seq 50); do (echo "hi $i" | nc -q1 localhost 2520 &); done
 *   Use `ps -L -p <server pid> | wc -l` to watch the thread count.
 *
 * QUESTIONS
 *   1. Why malloc a separate client_args_t per client instead of passing
 *      &client (a local in main's loop)? (Exercise 1, Part B.)
 *   2. Why detach instead of join here?
 *   3. Add a global "total bytes echoed" counter updated by every thread.
 *      Is it correct under the stress test? (Exercise 4 — you need
 *      coordination, coming soon.)
 *   4. One client sends data that triggers a bug causing a segfault in
 *      handle_client. What happens to all the OTHER clients? How would a
 *      fork-per-client server differ? (Lecture: drawbacks of threads.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g -pthread ex8_threaded_echo_server.c -o ex8
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
    int fd;
    int id;
    char peer[INET_ADDRSTRLEN + 8];
} client_args_t;

int listen_on( uint16_t port );
int sendall( int s, const char *buf, int len );
void *handle_client( void *arg );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int listen_on( uint16_t port ) { /* TODO (from L08 ex1) */ return -1; }
int sendall( int s, const char *buf, int len ) { /* TODO (from L08 ex2) */ return -1; }
void *handle_client( void *arg ) { /* TODO */ return NULL; }
