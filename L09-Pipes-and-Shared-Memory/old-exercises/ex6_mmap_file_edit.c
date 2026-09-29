/*
 * Lecture 9 — Exercise 6: mmap — Shared vs Private, msync, Guard Pages
 *
 * SETUP
 *   echo "The quick brown fox jumps over the lazy dog." > fox.txt
 *
 * PART A — edit a file as memory
 *   ./ex6 A fox.txt
 *   - open(O_RDWR), stat() for the size, mmap(NULL, size, PROT_READ |
 *     PROT_WRITE, MAP_SHARED, fd, 0). Check against MAP_FAILED.
 *   - Uppercase every letter IN PLACE with a plain loop over the char *
 *     (toupper) — no write() calls at all!
 *   - msync(MS_SYNC), munmap, close. `cat fox.txt` — it changed.
 *
 * PART B — shared vs private, across fork (like the lecture example)
 *   ./ex6 B fox.txt <shared|private>
 *   - Map the file, then fork. Child lowercases the whole mapping, msyncs,
 *     munmaps, exits. Parent waitpid()s, then prints the mapping AND
 *     re-reads the file with fopen/fgets and prints that too.
 *   - Fill in the table in the comment at the bottom for both flags:
 *       parent's view of memory | file contents on disk
 *   - Explain using the lecture's definitions of MAP_SHARED / MAP_PRIVATE.
 *
 * PART C — size can't change
 *   ./ex6 C fox.txt
 *   Map the file, then try to sprintf a message LONGER than the file into
 *   the mapping (use snprintf with the real size to be safe, then try an
 *   unchecked strcpy of a much longer string). What happens to the file?
 *   What about writing far past the end (e.g. index size + 5000)? Which
 *   signal? (SIGSEGV or SIGBUS.) Then write a SHORTER message after
 *   memset(0) like the lecture — what does `cat -v fox.txt` show?
 *
 * PART D — PROT_NONE guard page
 *   ./ex6 D
 *   - long page = sysconf(_SC_PAGESIZE);
 *   - Map 3 pages anonymously:
 *       mmap(NULL, 3*page, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0)
 *   - mprotect the FIRST and LAST page to PROT_NONE. The middle page is your
 *     "buffer".
 *   - Write to every byte of the middle page (fine), then write one byte
 *     past it. Observe the crash. This is how the lecture's "guards" catch
 *     buffer overruns immediately. (Catch SIGSEGV with a handler that
 *     write()s "guard page hit!" and _exit(1) for a friendlier demo.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex6_mmap_file_edit.c -o ex6
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

int part_a( const char *path );
int part_b( const char *path, int shared );
int part_c( const char *path );
int part_d( void );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int part_a( const char *path ) { /* TODO */ return 0; }
int part_b( const char *path, int shared ) { /* TODO */ return 0; }
int part_c( const char *path ) { /* TODO */ return 0; }
int part_d( void ) { /* TODO */ return 0; }

/*
 * PART B RESULTS
 *                 | parent sees in memory | file on disk afterwards
 *   MAP_SHARED    |                       |
 *   MAP_PRIVATE   |                       |
 */
