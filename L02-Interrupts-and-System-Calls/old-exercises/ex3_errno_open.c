/*
 * Lecture 2 — Exercise 3: When System Calls Fail
 *
 * GOAL
 *   System calls report failure by returning -1 and setting errno (Lecture 1).
 *   Learn to decode errno instead of printing a useless "-1".
 *
 * TASKS
 *   Implement try_open() so that it calls open(path, flags) and:
 *     - on success prints   OK   <path> -> fd <n>   and closes the fd
 *     - on failure prints   FAIL <path>: errno=<n> (<NAME>) <message>
 *       where <NAME> is the symbolic constant (ENOENT, EACCES, EISDIR, ...)
 *       and <message> comes from strerror(errno).
 *   Write errno_name() yourself with a switch over at least:
 *       ENOENT, EACCES, EISDIR, ENOTDIR, EEXIST, EMFILE
 *   (IMPORTANT: copy errno into a local variable immediately after the failing
 *    call — any later library call, even printf, is allowed to change it.)
 *
 *   Then trigger each of these on purpose from main:
 *     1. A file that doesn't exist.
 *     2. A file you don't have permission to read (create one with
 *          touch secret.txt && chmod 000 secret.txt ).
 *        (If you're root on WSL this won't fail — note that and explain why.)
 *     3. Opening a directory for writing:  open(".", O_WRONLY)
 *     4. A path where a component isn't a directory: "ex3_errno_open.c/foo"
 *     5. O_CREAT | O_EXCL on a file that already exists
 *        (needs a third argument: mode, e.g. 0644).
 *     6. Keep opening the same file WITHOUT closing until it fails. How many
 *        fds did you get? Compare with `ulimit -n`. Why is the count 3 less?
 *
 *   Finally, compare your output with perror("open") for one case.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex3_errno_open.c -o ex3
 *   ./ex3
 *
 * READ THE DOCS
 *   man 2 open   -> scroll to the ERRORS section. Every constant you print is
 *                   listed there with its meaning.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

const char *errno_name( int err );
void try_open( const char *path, int flags );

int main( void ) {
    /* TODO: cases 1-5 via try_open, then case 6 as a loop */
    return 0;
}

const char *errno_name( int err ) {
    /* TODO */
    return "UNKNOWN";
}

void try_open( const char *path, int flags ) {
    /* TODO */
}
