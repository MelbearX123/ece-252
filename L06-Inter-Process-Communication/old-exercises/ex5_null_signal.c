/*
 * Lecture 6 — Exercise 5: Is It Alive? (the null signal and pid files)
 *
 * GOAL
 *   Use kill(pid, 0) to test for a process's existence, interpret errno,
 *   and implement the "pid file" convention for finding a service.
 *
 * PART A — probe
 *   ./ex5 probe <pid>
 *   Call kill(pid, 0) and print one of:
 *     "<pid>: alive and signalable"          (returned 0)
 *     "<pid>: does not exist (ESRCH)"
 *     "<pid>: exists but not yours (EPERM)"
 *   Try: your shell's pid ($$), pid 1, a pid you just killed, a huge pid.
 *
 * PART B — a "service" with a pid file (like mysqld.pid in the lecture)
 *   ./ex5 serve
 *     - If "service.pid" exists AND the pid in it is alive, print
 *       "already running as <pid>" and exit 1 (single-instance check).
 *     - Otherwise write getpid() into "service.pid", install a SIGTERM
 *       handler that sets a flag, and loop printing "serving..." every 2s
 *       until the flag is set. On exit, remove("service.pid").
 *   ./ex5 stop
 *     - Read the pid from service.pid, send SIGTERM with kill(), then poll
 *       kill(pid, 0) every 100 ms until it fails with ESRCH (max 5 s).
 *       Print "stopped" or "did not stop, sending SIGKILL" and escalate.
 *
 * TEST
 *   T1: ./ex5 serve        T2: ./ex5 serve   (refused)   T2: ./ex5 stop
 *   Then: kill -9 the server so it can't remove its pid file. Run
 *   ./ex5 serve again — does your stale-pid-file check handle it?
 *
 * QUESTIONS
 *   1. The lecture lists two reasons the null signal is of limited use.
 *      Describe a concrete bad scenario for each with the pid-file design.
 *   2. What would kill(0, SIGTERM) do? kill(-1, SIGTERM)? (DON'T run the
 *      second one — reason about it.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex5_null_signal.c -o ex5
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#define PIDFILE "service.pid"

int probe( pid_t pid );
int serve( void );
int stop( void );

int main( int argc, char **argv ) {
    /* TODO: dispatch */
    return 0;
}

int probe( pid_t pid ) { /* TODO */ return 0; }
int serve( void ) { /* TODO */ return 0; }
int stop( void ) { /* TODO */ return 0; }
