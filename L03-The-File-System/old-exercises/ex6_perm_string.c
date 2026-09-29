/*
 * Lecture 3 — Exercise 6: Permission Strings <-> Octal
 *
 * GOAL
 *   Make UNIX permission bits second nature by converting both ways, and
 *   apply the owner > group > others precedence rule.
 *
 * TASKS
 *   1. octal_to_string(mode, is_dir, out):
 *        0750, is_dir=0  -> "-rwxr-x---"
 *        0644, is_dir=1  -> "drw-r--r--"
 *      out must hold 11 chars (10 + '\0').
 *      Use bitwise AND with masks: owner read is 0400, owner write 0200,
 *      owner exec 0100, group read 0040, ... others exec 0001.
 *      (Hint: loop over 9 bits from 0400 downward, shifting the mask right.)
 *
 *   2. string_to_octal(str) -> int:
 *        "-rwxr-----" -> 0740
 *      Return -1 if str is not exactly 10 chars or contains an invalid
 *      character in any position.
 *
 *   3. can_access(mode, file_owner, file_group, user, user_group, want):
 *      want is 'r', 'w', or 'x'. Return 1 if allowed, 0 otherwise, applying
 *      the lecture's precedence rule: if user == owner, ONLY owner bits
 *      apply; else if user_group == file_group, ONLY group bits; else others.
 *      Tricky case to include in tests: mode 0077 — the owner has NO access
 *      even though everyone else does.
 *
 *   4. main: run the tests in the table below and print PASS/FAIL for each.
 *      Then accept optional argv: ./ex6 755  -> prints "-rwxr-xr-x"
 *      (parse octal with strtol(argv[1], NULL, 8)).
 *
 * STRETCH
 *   Given a real file path, call stat() and print its permissions with your
 *   function; compare to `ls -l` output.
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex6_perm_string.c -o ex6
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void octal_to_string( int mode, int is_dir, char *out );
int string_to_octal( const char *str );
int can_access( int mode, int file_owner, int file_group,
                int user, int user_group, char want );

int main( int argc, char **argv ) {
    /*
     * TESTS
     *   octal_to_string(0750,0) == "-rwxr-x---"
     *   octal_to_string(0644,1) == "drw-r--r--"
     *   octal_to_string(0000,0) == "----------"
     *   octal_to_string(0777,0) == "-rwxrwxrwx"
     *   string_to_octal("-rwxr-----") == 0740
     *   string_to_octal("drwxr-xr-x") == 0755
     *   string_to_octal("-rwz------") == -1
     *   can_access(0640, 10, 5, 10, 5, 'w') == 1   owner write
     *   can_access(0640, 10, 5, 11, 5, 'w') == 0   group has r only
     *   can_access(0640, 10, 5, 11, 5, 'r') == 1
     *   can_access(0640, 10, 5, 12, 9, 'r') == 0   others nothing
     *   can_access(0077, 10, 5, 10, 5, 'r') == 0   owner locked out!
     *   can_access(0077, 10, 5, 12, 9, 'x') == 1
     */
    /* TODO */
    return 0;
}

void octal_to_string( int mode, int is_dir, char *out ) { /* TODO */ }
int string_to_octal( const char *str ) { /* TODO */ return -1; }
int can_access( int mode, int file_owner, int file_group,
                int user, int user_group, char want ) { /* TODO */ return 0; }
