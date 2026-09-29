/*
 * Lecture 8 — Exercise 6: libcurl GET with a Write Callback
 *
 * GOAL
 *   Use the easy interface lifecycle (global init -> easy init -> setopt ->
 *   perform -> check -> cleanup -> global cleanup) and write callbacks
 *   that receive data in chunks.
 *
 * PART A — minimal
 *   ./ex6 A <url>
 *   Reproduce the lecture's basic example (no callback): the body goes to
 *   stdout. Check every CURLcode; on failure print curl_easy_strerror.
 *   Try a bad URL (https://no.such.host.invalid) and read the error text.
 *
 * PART B — into memory
 *   ./ex6 B <url>
 *   - Define   typedef struct { char *data; size_t len; size_t cap; int calls; } membuf_t;
 *   - write_to_mem(ptr, size, nmemb, userdata):
 *       * n = size * nmemb
 *       * grow data with realloc if len + n + 1 > cap (double the cap)
 *       * memcpy the chunk in, len += n, data[len] = '\0', calls++
 *       * return n   (return anything else and curl aborts — try returning
 *                     0 once and see which CURLcode you get)
 *   - CURLOPT_WRITEFUNCTION = write_to_mem, CURLOPT_WRITEDATA = &buf
 *   - After perform, print:
 *       "HTTP <code>, <len> bytes in <calls> callback calls"
 *     using curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code) (long).
 *     Then print the contents of the <title>...</title> tag if present.
 *   - free the buffer.
 *
 * PART C — straight to a file, reusing the handle
 *   ./ex6 C <url1> <url2> ...
 *   - ONE easy handle for all URLs (lecture: handles can be reused).
 *   - For each URL i, fopen "page_<i>.html" and pass the FILE* as
 *     CURLOPT_WRITEDATA with a write_to_file callback that uses fwrite and
 *     returns what fwrite wrote. Close each file.
 *   - Use CURLOPT_FOLLOWLOCATION = 1L so redirects are followed.
 *   - Between URLs, only CURLOPT_URL and CURLOPT_WRITEDATA change. When
 *     would you need curl_easy_reset instead?
 *
 * TRY
 *   ./ex6 B https://example.com/
 *   ./ex6 B https://www.gnu.org/licenses/gpl-3.0.txt   (big: many calls)
 *   ./ex6 C https://example.com/ http://uwaterloo.ca/
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex6_curl_get_to_memory.c -o ex6 -lcurl
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

typedef struct {
    char *data;
    size_t len;
    size_t cap;
    int calls;
} membuf_t;

size_t write_to_mem( char *ptr, size_t size, size_t nmemb, void *userdata );
size_t write_to_file( char *ptr, size_t size, size_t nmemb, void *userdata );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

size_t write_to_mem( char *ptr, size_t size, size_t nmemb, void *userdata ) { /* TODO */ return 0; }
size_t write_to_file( char *ptr, size_t size, size_t nmemb, void *userdata ) { /* TODO */ return 0; }
