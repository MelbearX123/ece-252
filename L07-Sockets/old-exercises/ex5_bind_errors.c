/*
 * Lecture 7 — Exercise 5: Making Sockets Fail on Purpose
 *
 * GOAL
 *   Learn the common socket errors by causing each one, reading errno,
 *   and matching it to the man pages (man 2 bind, man 2 connect,
 *   man 2 listen).
 *
 * Write a small test for each case. Each test prints:
 *   "<test name>: <call> returned <ret>, errno=<n> (<strerror>)"
 * and whether that matches the EXPECTED constant (compare errno ==
 * EADDRINUSE etc. — never hard-code numbers).
 *
 *   T1  Bind the same port twice in the same process
 *       (two sockets, both bind to 2520)               expect EADDRINUSE
 *   T2  Bind to a privileged port, e.g. 80, as a
 *       normal user                                    expect EACCES
 *       (if you're root in WSL, it will succeed — explain why)
 *   T3  connect() to localhost on a port where nobody
 *       is listening                                   expect ECONNREFUSED
 *   T4  accept() on a socket that was bound but never
 *       listen()ed                                     expect EINVAL
 *   T5  bind() a socket twice (same socket, two ports) expect EINVAL
 *   T6  connect() with sin_port = 2520 WITHOUT htons,
 *       while your ex3 server runs on 2520             -> which port did it
 *       really try? (it's refused — compute the real port and run
 *       `nc -l <that port>` to prove it connects there instead!)
 *   T7  The TIME_WAIT effect: start ex3 without SO_REUSEADDR, connect with
 *       nc, stop the server with Ctrl-C, restart immediately.
 *       Which errno? Why does the OS hold the port for a while?
 *       (Do this one manually; just write the result below.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex5_bind_errors.c -o ex5
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

/* Helper: fill a loopback IPv4 address for the given port (already in host order). */
static struct sockaddr_in make_addr( uint16_t port_host_order ) {
    struct sockaddr_in a;
    memset( &a, 0, sizeof( a ) );
    a.sin_family = AF_INET;
    a.sin_port = htons( port_host_order );
    a.sin_addr.s_addr = htonl( INADDR_LOOPBACK );
    return a;
}

static void report( const char *test, const char *call, int ret, int err, int expected ) {
    printf( "%-4s %-8s returned %d, errno=%d (%s) -> %s\n",
            test, call, ret, err, strerror( err ),
            err == expected ? "as expected" : "UNEXPECTED" );
}

int main( void ) {
    /* TODO: T1..T6 */
    (void) make_addr; (void) report;
    return 0;
}

/*
 * T7 RESULT:
 */
