/*
 * Lecture 7 — Exercise 2: A Mini nslookup with getaddrinfo
 *
 * GOAL
 *   Use getaddrinfo/freeaddrinfo, walk the result linked list, and pull
 *   the IP and port out of a struct sockaddr_in.
 *
 * SPEC
 *   ./ex2 <hostname> [port] [tcp|udp]
 *     e.g. ./ex2 uwaterloo.ca
 *          ./ex2 www.google.com 443
 *          ./ex2 localhost 2520 udp
 *          ./ex2 no.such.host.invalid
 *
 *   - Set up hints: ai_family = AF_INET, ai_socktype = SOCK_STREAM (or
 *     SOCK_DGRAM for "udp"). Port defaults to "80".
 *   - If getaddrinfo returns non-zero, print
 *       "lookup failed: <gai_strerror(result)>"  and return 1.
 *   - Walk the list using ai_next. For each node print:
 *       "#<i>  <ip>:<port>  socktype=<STREAM|DGRAM> protocol=<n>"
 *     where:
 *       * cast ai_addr to struct sockaddr_in *
 *       * convert sin_addr to text with inet_ntop(AF_INET, &sa->sin_addr,
 *         buf, INET_ADDRSTRLEN)
 *       * port: remember sin_port is in NETWORK order -> ntohs
 *   - freeaddrinfo at the end (run valgrind to confirm no leak).
 *
 * EXPERIMENTS
 *   1. Compare your output to `nslookup uwaterloo.ca` or `getent hosts
 *      uwaterloo.ca`.
 *   2. Print the port WITHOUT ntohs. What number do you get for 80? For
 *      2520? Explain using Exercise 1.
 *   3. Pass hints = NULL. How many results do you get now and why?
 *      (TCP + UDP + raw? IPv6 too? Print ai_family.)
 *   4. Use node = NULL with ai_flags = AI_PASSIVE. What address comes
 *      back? Why is that what a server wants? Remove AI_PASSIVE — now?
 *   5. Use "http" as the service instead of "80". Does it work? Where does
 *      the mapping come from? (Look at /etc/services.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex2_lookup.c -o ex2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}
