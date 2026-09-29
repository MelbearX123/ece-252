/*
 * Lecture 5 — Exercise 4: Make a Zombie, Make an Orphan
 *
 * GOAL
 *   See the Lecture 4 concepts (zombie, orphan, reaping, adoption by init)
 *   happen for real.
 *
 * PART Z — zombie
 *   ./ex4 Z
 *   - Fork. The child prints its pid and exits immediately with code 7.
 *   - The parent prints "parent sleeping, child <pid> should be a zombie"
 *     and sleep(20)s WITHOUT calling wait.
 *   - While it sleeps, in another terminal:
 *         ps -o pid,ppid,stat,cmd -p <childpid>
 *     You should see STAT 'Z' and "<defunct>". Try `kill -9 <childpid>` —
 *     does it go away? Why not? (You can't kill what's already dead.)
 *   - After sleeping, the parent calls wait(&status), prints the exit code
 *     via WEXITSTATUS, then sleeps 10 more seconds. Check ps again — the
 *     zombie should be gone (reaped).
 *
 * PART O — orphan
 *   ./ex4 O
 *   - Fork. The parent prints its pid and exits IMMEDIATELY (no wait).
 *   - The child prints getppid() once per second for 5 seconds.
 *   - Q: What does getppid() print after the parent dies? Is it 1? On
 *     modern Linux it might be a "subreaper" (e.g. systemd --user) instead
 *     of init — check what that pid is with ps -p <pid>.
 *   - Q: Why does your shell prompt come back while the child is still
 *     printing?
 *
 * PART M — many zombies
 *   ./ex4 M
 *   - Fork 5 children that each exit immediately; parent sleeps 15s, then
 *     reaps them all in a loop ( while ((pid = wait(&st)) > 0) ... ) and
 *     prints each pid + code. Watch `ps` during the sleep.
 *   - Q: In what ORDER does wait return them? Is it guaranteed?
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex4_zombie_orphan.c -o ex4
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void zombie( void );
void orphan( void );
void many_zombies( void );

int main( int argc, char **argv ) {
    if ( argc != 2 ) {
        printf( "Usage: %s Z | O | M\n", argv[0] );
        return -1;
    }
    switch ( argv[1][0] ) {
        case 'Z': zombie(); break;
        case 'O': orphan(); break;
        case 'M': many_zombies(); break;
        default: return -1;
    }
    return 0;
}

void zombie( void ) { /* TODO */ }
void orphan( void ) { /* TODO */ }
void many_zombies( void ) { /* TODO */ }
