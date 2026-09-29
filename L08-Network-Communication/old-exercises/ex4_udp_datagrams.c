/*
 * Lecture 8 — Exercise 4: UDP Datagrams
 *
 * GOAL
 *   Use SOCK_DGRAM with sendto/recvfrom, and experience the differences
 *   from TCP: no connection, no accept, message boundaries preserved,
 *   delivery not guaranteed.
 *
 * SPEC — one program, several modes
 *   ./ex4 recv <port>
 *     - socket(AF_INET, SOCK_DGRAM, 0), bind to INADDR_ANY:<port>
 *       (no listen, no accept!)
 *     - loop: n = recvfrom(fd, buf, sizeof buf - 1, 0, &from, &fromlen)
 *       print "[<ip>:<port>] <n> bytes: <text>"
 *       then reply to THAT sender with sendto(..., &from, fromlen):
 *       "ack <sequence number you parsed from the message>"
 *
 *   ./ex4 send <host> <port> <count>
 *     - getaddrinfo(host, port, hints{AF_INET, SOCK_DGRAM})
 *     - socket, then for i in 0..count-1: sendto "msg <i>" to the address
 *       (no connect!), then recvfrom one ack with a 1-second timeout:
 *           struct timeval tv = {1, 0};
 *           setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
 *       If recvfrom fails with EAGAIN/EWOULDBLOCK, print "msg <i> LOST
 *       (no ack)".
 *
 *   ./ex4 csend <host> <port> <count>
 *     - Same, but call connect() on the UDP socket first and then use plain
 *       send()/recv(). (Lecture: still UDP, just a default destination.)
 *
 * EXPERIMENTS
 *   1. Run the sender with NO receiver running. Does sendto fail? Compare
 *      with TCP connect() in Lecture 7 (ECONNREFUSED). Then try csend with
 *      no receiver — recv may now report ECONNREFUSED. Why the difference?
 *      (The connected UDP socket can receive the ICMP "port unreachable".)
 *   2. Send "AAAA", "BBBB", "CCCC" quickly, and have the receiver sleep(2)
 *      before its first recvfrom. Does it get 3 separate messages or one
 *      glued-together chunk? Compare to what TCP would do (Exercise 1/2).
 *   3. Make the receiver's buffer 3 bytes. What happens to the rest of a
 *      10-byte datagram? (It's discarded — datagrams are all-or-nothing
 *      per recvfrom.)
 *   4. Simulate loss: in the receiver, randomly skip replying to 20% of
 *      messages. Does your sender correctly report LOST ones?
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex4_udp_datagrams.c -o ex4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int run_receiver( const char *port );
int run_sender( const char *host, const char *port, int count, int use_connect );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int run_receiver( const char *port ) { /* TODO */ return 0; }
int run_sender( const char *host, const char *port, int count, int use_connect ) { /* TODO */ return 0; }
