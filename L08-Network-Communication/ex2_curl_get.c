/*
 * Lecture 8 — Exercise 2: Download a Page with libcurl
 *
 * GOAL
 *   Use the easy interface: global init -> easy init -> setopt ->
 *   perform -> cleanup.
 *
 * SPEC
 *   ./ex2 <url>
 *   - curl_global_init( CURL_GLOBAL_DEFAULT ), then curl_easy_init().
 *   - Set CURLOPT_URL to argv[1].
 *   - Set CURLOPT_WRITEFUNCTION to write_cb. write_cb just adds
 *     size * nmemb to the total pointed to by userdata and returns
 *     size * nmemb. Set CURLOPT_WRITEDATA to &total.
 *   - curl_easy_perform(). If it doesn't return CURLE_OK, print
 *     curl_easy_strerror( res ).
 *   - Print "downloaded <total> bytes", then clean up.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex2_curl_get.c -o ex2 -lcurl
 *   ./ex2 https://example.com/
 */

#include <stdio.h>
#include <curl/curl.h>

size_t write_cb( char *ptr, size_t size, size_t nmemb, void *userdata );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

size_t write_cb( char *ptr, size_t size, size_t nmemb, void *userdata ) {
    /* TODO */
    return 0;
}
