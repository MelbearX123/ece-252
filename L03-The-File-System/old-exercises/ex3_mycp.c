/*
 * Lecture 3 — Exercise 3: Build cp and mv from the Basic Operations
 *
 * GOAL
 *   The lecture says higher-level operations are compositions of the six
 *   basic ones: copy = create + read + write; move = copy + delete.
 *   Build them.
 *
 * SPEC
 *   ./ex3 cp <src> <dst>        copy src to dst (dst created or truncated)
 *   ./ex3 cp -n <src> <dst>     same, but refuse if dst already exists
 *   ./ex3 mv <src> <dst>        copy, then remove(src) only if the copy fully succeeded
 *
 *   - Copy in binary-safe chunks: open both with "rb"/"wb" and use
 *     fread/fwrite with a 4096-byte buffer (fread returns the count read;
 *     0 means EOF or error — use ferror() to tell which).
 *   - For -n, use the C11 "x" mode flag ("wbx") so the check-and-create is
 *     a single operation. Why is "check with fopen(dst,"r") first, then
 *     create" worse? (Think about two programs doing it at the same time.)
 *   - Refuse to copy a file onto itself (compare the paths — or stretch:
 *     compare st_ino from stat()).
 *   - Print useful errors with perror and return non-zero on failure.
 *
 * COMPILE / TEST
 *   gcc -Wall -Wextra -g ex3_mycp.c -o ex3
 *   ./ex3 cp ex3_mycp.c copy.c && cmp ex3_mycp.c copy.c && echo SAME
 *   ./ex3 cp ex3 ex3_copy && ./ex3_copy     (binary copy still runs? chmod +x may be needed — why?)
 *   ./ex3 cp -n ex3_mycp.c copy.c           (should refuse)
 *   ./ex3 mv copy.c moved.c && ls
 *
 * THINK ABOUT
 *   - After ./ex3 cp ex3 ex3_copy, does ex3_copy have execute permission?
 *     Why not? Which file attribute did your copy NOT preserve?
 *     (Stretch: use stat() + chmod() to copy the permission bits too.)
 *   - Real `mv` on the same file system does NOT copy data at all. What does
 *     it do instead? (Hint: which directory operation from the lecture?)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK 4096

/* Returns 0 on success, -1 on failure. no_clobber != 0 means use "x" mode. */
int copy_file( const char *src, const char *dst, int no_clobber );

int main( int argc, char **argv ) {
    /* TODO: parse cp / cp -n / mv */
    return 0;
}

int copy_file( const char *src, const char *dst, int no_clobber ) {
    /* TODO */
    return 0;
}
