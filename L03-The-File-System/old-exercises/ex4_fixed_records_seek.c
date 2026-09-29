/*
 * Lecture 3 — Exercise 4: Random Access with fseek
 *
 * GOAL
 *   Use repositioning to jump straight to the Nth record of a file instead
 *   of reading everything before it — and update a record in place.
 *
 * FILE FORMAT
 *   "students.db" is a text file of fixed-width records, each exactly
 *   RECORD_LEN bytes including the trailing '\n':
 *
 *     id(5 digits) space name(20 chars, space padded) space grade(3 digits) '\n'
 *     e.g. "00007 Ada Lovelace         097\n"
 *
 *   Because every record has the same length, record k starts at byte
 *   k * RECORD_LEN.
 *
 * COMMANDS
 *   ./ex4 init                  create students.db with 10 sample records ("w")
 *   ./ex4 get <k>               fseek to record k, read it, print it
 *   ./ex4 set <k> <grade>       open "r+", fseek to the grade field of record k,
 *                               overwrite ONLY those 3 bytes with fprintf("%03d")
 *   ./ex4 dump                  print all records
 *
 * REQUIREMENTS
 *   - get/set must NOT read the records before k. Use fseek(f, offset, SEEK_SET).
 *   - Validate k: use fseek(f, 0, SEEK_END) + ftell(f) to get the file size,
 *     then number of records = size / RECORD_LEN.
 *   - After `set`, run `dump` and confirm only one grade changed.
 *
 * THINK ABOUT
 *   - What happens if you open with "a" for `set` instead of "r+"? Try it.
 *   - What goes wrong if names could be variable-length? How would you then
 *     find record k? (This is why databases use fixed-size pages/indexes.)
 *   - Read 1 record, then fseek(f, RECORD_LEN, SEEK_CUR). Which record are
 *     you now positioned at? (Lecture: read 48 + seek 48 = 96.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex4_fixed_records_seek.c -o ex4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DBFILE     "students.db"
#define NAME_LEN   20
#define RECORD_LEN ( 5 + 1 + NAME_LEN + 1 + 3 + 1 )   /* = 31 */
#define GRADE_OFFSET ( 5 + 1 + NAME_LEN + 1 )          /* offset of grade within a record */

int db_init( void );
int db_get( int k );
int db_set( int k, int grade );
int db_dump( void );

int main( int argc, char **argv ) {
    /* TODO: dispatch on argv[1] */
    return 0;
}

int db_init( void ) {
    /* TODO: write 10 records with fprintf(f, "%05d %-20s %03d\n", id, name, grade) */
    return 0;
}

int db_get( int k ) { /* TODO */ return 0; }
int db_set( int k, int grade ) { /* TODO */ return 0; }
int db_dump( void ) { /* TODO */ return 0; }
