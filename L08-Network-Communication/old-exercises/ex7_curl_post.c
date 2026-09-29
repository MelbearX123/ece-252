/*
 * Lecture 8 — Exercise 7: libcurl POST with a Read Callback
 *
 * GOAL
 *   Upload data using a read callback that is called repeatedly in
 *   chunks, and capture the server's reply with a write callback — both
 *   directions on one handle.
 *
 * SPEC
 *   ./ex7 <file_to_upload> [url]
 *   url defaults to https://httpbin.org/post (a public echo service that
 *   replies with JSON describing what you sent).
 *
 *   - Read the whole file into a heap buffer (fopen/fseek/ftell/fread or a
 *     read loop).
 *   - Set up struct upload { const char *readptr; size_t sizeleft;
 *                            int calls; size_t max_chunk; }.
 *   - read_callback(dest, size, nmemb, userp): copy
 *     min(sizeleft, size*nmemb) bytes with memcpy, advance readptr,
 *     decrease sizeleft, count calls, record the largest buffer curl
 *     offered (size*nmemb), return bytes copied; return 0 when done.
 *   - Options:
 *       CURLOPT_URL
 *       CURLOPT_POST, 1L
 *       CURLOPT_READFUNCTION / CURLOPT_READDATA
 *       CURLOPT_POSTFIELDSIZE, (long) file_size   (tells curl the total so
 *           it can send a Content-Length header instead of chunked encoding)
 *       CURLOPT_WRITEFUNCTION / CURLOPT_WRITEDATA  (reuse membuf_t from ex6)
 *   - Perform, check CURLcode, print:
 *       "uploaded <n> bytes in <calls> read callbacks (curl buffer <max_chunk> bytes)"
 *       "HTTP <status>"  and the first 500 chars of the response.
 *   - Verify the response JSON contains your file's first line.
 *   - Clean up everything (valgrind: no leaks).
 *
 * EXPERIMENTS
 *   1. Upload a tiny file and a ~1 MB file. How many read callbacks each?
 *      What's the buffer size curl gives you? (Relate to "we might need to
 *      do this in several chunks" from the lecture.)
 *   2. Make the read callback return one byte fewer than it copied. What
 *      happens? What about returning more than size*nmemb? (Don't — read
 *      the docs for CURL_READFUNC_ABORT instead.)
 *   3. Remove CURLOPT_POSTFIELDSIZE. Use CURLOPT_VERBOSE, 1L to see the
 *      headers curl sends. What changed? (Transfer-Encoding: chunked.)
 *   4. Rewrite using CURLOPT_POSTFIELDS with the whole buffer instead of a
 *      read callback. Why might the callback version be better for a 4 GB
 *      file?
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex7_curl_post.c -o ex7 -lcurl
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct upload {
    const char *readptr;
    size_t sizeleft;
    int calls;
    size_t max_chunk;
};

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} membuf_t;

size_t read_callback( char *dest, size_t size, size_t nmemb, void *userp );
size_t write_to_mem( char *ptr, size_t size, size_t nmemb, void *userdata );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

size_t read_callback( char *dest, size_t size, size_t nmemb, void *userp ) { /* TODO */ return 0; }
size_t write_to_mem( char *ptr, size_t size, size_t nmemb, void *userdata ) { /* TODO */ return 0; }
