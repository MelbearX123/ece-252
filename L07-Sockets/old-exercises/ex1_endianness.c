/*
 * Lecture 7 — Exercise 1: Endianness
 *
 * GOAL
 *   See byte order with your own eyes and understand exactly what htonl /
 *   htons / ntohl / ntohs do.
 *
 * TASKS
 *   1. print_bytes(label, ptr, n): print the n bytes starting at ptr in
 *      memory order as hex, e.g.  "0x0A0B0C0D in memory: 0d 0c 0b 0a".
 *      (Cast the pointer to unsigned char * to walk byte by byte.)
 *   2. is_little_endian(): return 1 if the host is little-endian. Do it by
 *      storing uint32_t x = 1 and inspecting the FIRST byte in memory.
 *   3. my_htonl(x) and my_htons(x): implement using shifts and masks ONLY
 *      (no library calls). They must be no-ops on big-endian hosts
 *      (use is_little_endian()).
 *   4. main:
 *      - print "Host is little-endian" / "big-endian"
 *      - print_bytes of 0x0A0B0C0D, htonl(0x0A0B0C0D), my_htonl(0x0A0B0C0D)
 *      - verify my_htonl(v) == htonl(v) and my_htons(p) == htons(p) for
 *        v in {0, 1, 0x12345678, 0xFFFFFFFF, 0x0A0B0C0D} and
 *        p in {0, 80, 2520, 65535}. Print PASS/FAIL.
 *      - verify ntohl(htonl(v)) == v (round trip).
 *   5. The port bug: print htons(2520) as a plain decimal. Now imagine you
 *      forgot htons and set sin_port = 2520 on this machine. Which port
 *      would the OTHER side actually see? Compute it and print it.
 *
 * QUESTIONS
 *   - Why don't we need to convert a single char, or a string?
 *   - Why is there no hton for double? How would you send a double?
 *     (Preview of serialization in Lecture 8.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex1_endianness.c -o ex1
 */

#include <stdio.h>
#include <stdint.h>
#include <arpa/inet.h>

void print_bytes( const char *label, const void *ptr, size_t n );
int is_little_endian( void );
uint32_t my_htonl( uint32_t x );
uint16_t my_htons( uint16_t x );

int main( void ) {
    /* TODO */
    return 0;
}

void print_bytes( const char *label, const void *ptr, size_t n ) { /* TODO */ }
int is_little_endian( void ) { /* TODO */ return 0; }
uint32_t my_htonl( uint32_t x ) { /* TODO */ return x; }
uint16_t my_htons( uint16_t x ) { /* TODO */ return x; }
