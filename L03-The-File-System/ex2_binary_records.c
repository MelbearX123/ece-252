/*
 * Lecture 3 — Exercise 2: Binary Records
 *
 * GOAL
 *   Write structs to a binary file with fwrite, read them back with fread,
 *   and jump straight to one record with fseek.
 *
 * TASKS
 *   1. write_students(): open "students.bin" in "wb" mode and write the
 *      whole students[] array with ONE call:
 *        fwrite( students, sizeof( student_t ), NUM_STUDENTS, fp )
 *   2. read_all(): open it in "rb" mode and loop while
 *        fread( &s, sizeof( student_t ), 1, fp ) == 1
 *      printing "id: <id>, grade: <grade>" (grade with 1 decimal place).
 *   3. read_one( k ): open in "rb" mode, fseek to the start of record k
 *      (record k starts k * sizeof( student_t ) bytes in, from SEEK_SET),
 *      fread ONE student and print it.
 *   4. In all three functions: check fopen for NULL, and fclose at the end.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex2_binary_records.c -o ex2
 *   ./ex2
 *
 * EXPECTED OUTPUT
 *   id: 101, grade: 88.5
 *   id: 102, grade: 92.0
 *   id: 103, grade: 75.5
 *   id: 104, grade: 60.0
 *   record 2 -> id: 103, grade: 75.5
 *
 * THINK ABOUT
 *   Try  cat students.bin  — why is it unreadable, unlike results.dat
 *   from ex1? Then try  ls -l students.bin  — it's 64 bytes, so each
 *   record is 16. But an int (4) + a double (8) is only 12. Where did
 *   the extra 4 bytes come from? (Look up "struct padding".)
 */

#include <stdio.h>

#define FILENAME "students.bin"
#define NUM_STUDENTS 4

typedef struct {
    int id;
    double grade;
} student_t;

void write_students( void );
void read_all( void );
void read_one( int k );

int main( void ) {
    write_students();
    read_all();
    read_one( 2 );
    return 0;
}

void write_students( void ) {
    student_t students[NUM_STUDENTS] = {
        { 101, 88.5 },
        { 102, 92.0 },
        { 103, 75.5 },
        { 104, 60.0 },
    };
    /* TODO 1 */
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        return;
    }
    fwrite( students, sizeof( student_t ), NUM_STUDENTS, fp );
    fclose(fp);
}

void read_all( void ) {
    /* TODO 2 */
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return;
    }
    student_t s;
    while (fread( &s, sizeof( student_t ), 1, fp ) == 1) {
        printf("id: %d, grade: %.1f\n", s.id, s.grade);
    }
    fclose(fp);
}

void read_one( int k ) {
    /* TODO 3 */
//  3. read_one( k ): open in "rb" mode, fseek to the start of record k
//  (record k starts k * sizeof( student_t ) bytes in, from SEEK_SET),
//  fread ONE student and print it.
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) {
        return;
    }
    fseek(fp, SEEK_SET, k*sizeof(student_t));
    student_t s;
    if (fread(&s, sizeof(student_t), 1, fp) != 1) {
        fclose(fp);
        return;
    }
    printf("record %d -> id: %d, grade: %.1f\n", k, s.id, s.grade);
    fclose(fp);
}
