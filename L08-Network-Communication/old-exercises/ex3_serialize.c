/*
 * Lecture 8 — Exercise 3: Serialization
 *
 * GOAL
 *   Understand why you shouldn't send(&my_struct, sizeof(my_struct)) and
 *   write a portable serializer/deserializer by hand (then appreciate why
 *   real code uses a library like protobuf-c).
 *
 * THE DATA
 *   typedef struct {
 *       uint32_t id;
 *       int16_t  temperature_tenths;   (e.g. -153 = -15.3 degrees)
 *       char     name[16];             (null-terminated, may be shorter)
 *       double   reading;
 *       uint8_t  flags;
 *   } sensor_t;
 *
 * PART A — the naive way is fragile
 *   Print sizeof(sensor_t) and the offset of every field (offsetof from
 *   <stddef.h>). Add up the field sizes by hand. Why is sizeof bigger?
 *   (Padding.) List three ways the raw bytes of this struct could differ
 *   between two machines.
 *
 * PART B — serialize into a byte buffer, fixed wire format:
 *     offset 0  : id                 4 bytes, big-endian (htonl)
 *     offset 4  : temperature_tenths 2 bytes, big-endian (htons on the uint16_t bits)
 *     offset 6  : name length N      1 byte  (0..15)
 *     offset 7  : name               N bytes, NO terminator
 *     then      : reading            as TEXT: 1-byte length + "%.6f" chars
 *                 (sending text sidesteps the double-format problem)
 *     then      : flags              1 byte
 *   size_t serialize( const sensor_t *s, uint8_t *buf, size_t cap );
 *     returns bytes written, or 0 if cap is too small.
 *   int deserialize( sensor_t *out, const uint8_t *buf, size_t len );
 *     returns 0 on success, -1 if the buffer is malformed/truncated.
 *     (Check EVERY length against len before reading — the network can lie.)
 *   Use memcpy to move multi-byte values in/out of the buffer (unaligned
 *   pointer casts like *(uint32_t*)(buf+1) are undefined behaviour).
 *
 * PART C — test
 *   Round-trip 3 sensors (including a negative temperature, a 15-char name,
 *   an empty name) and assert every field matches. Print the serialized
 *   bytes in hex for one of them and annotate which bytes are which.
 *   Feed deserialize a truncated buffer (len - 1) and garbage — it must
 *   return -1, not crash (run under valgrind / -fsanitize=address).
 *
 * PART D (optional) — over the wire
 *   Reuse Exercise 2's send_msg/recv_msg to send the serialized bytes from a
 *   forked child to the parent and deserialize there.
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex3_serialize.c -o ex3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <arpa/inet.h>

typedef struct {
    uint32_t id;
    int16_t temperature_tenths;
    char name[16];
    double reading;
    uint8_t flags;
} sensor_t;

size_t serialize( const sensor_t *s, uint8_t *buf, size_t cap );
int deserialize( sensor_t *out, const uint8_t *buf, size_t len );

int main( void ) {
    /* TODO: Part A, then Part C tests */
    return 0;
}

size_t serialize( const sensor_t *s, uint8_t *buf, size_t cap ) { /* TODO */ return 0; }
int deserialize( sensor_t *out, const uint8_t *buf, size_t len ) { /* TODO */ return -1; }
