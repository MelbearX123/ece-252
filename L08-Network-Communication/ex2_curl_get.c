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
    if (argc != 2) {
        return 1;
    }
    char *path = argv[1];
    CURL *curl;
    CURLcode res;
    ssize_t total = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, path);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &total);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            perror("curl error");
        } else {
            printf("downloaded %zu bytes\n", total); 
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}

size_t write_cb( char *ptr, size_t size, size_t nmemb, void *userdata ) {
    size_t chunk = size * nmemb;          // how many bytes arrived this time
    size_t *total = (size_t *) userdata;  // userdata is really &total from main
    *total += chunk;                      // add to main's counter
    return chunk;       
}
