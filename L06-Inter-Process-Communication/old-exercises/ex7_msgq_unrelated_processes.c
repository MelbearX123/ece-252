/*
 * Lecture 6 — Exercise 7: Two Unrelated Programs, One Queue (ftok)
 *
 * GOAL
 *   IPC_PRIVATE only works for parent/child. Use ftok() so two processes
 *   started separately (in two terminals) find the SAME queue — and build a
 *   tiny request/reply protocol using mtype to route messages.
 *
 * SPEC — one program, three modes
 *   ./ex7 server
 *     - key = ftok("ex7_msgq_unrelated_processes.c", 'Q')   (file must exist!)
 *     - msgget(key, 0600 | IPC_CREAT | IPC_EXCL). If it fails with EEXIST,
 *       print "server already running (or stale queue: ipcrm -q ...)" and exit.
 *     - Loop: msgrcv with type = 1 (requests are always mtype 1).
 *       A request contains: client pid, and a line of text.
 *       Reply with the text UPPERCASED, sent with mtype = <client pid>, so
 *       only that client picks it up.
 *       If the text is "shutdown", remove the queue (IPC_RMID) and exit.
 *   ./ex7 client "<text>"
 *     - Same ftok args; msgget(key, 0) (don't create — the server must
 *       exist; report ENOENT nicely).
 *     - Send {mtype=1, pid=getpid(), text}.
 *     - msgrcv with type = getpid() and print the reply.
 *   ./ex7 flood <n>
 *     - Fork n children that are each clients sending "hello from <pid>".
 *       Every child must get ITS OWN reply back. Verify.
 *
 * QUESTIONS
 *   1. Why does routing replies by mtype = pid work? What would go wrong if
 *      all replies used mtype 2 and multiple clients were waiting?
 *   2. What happens to a client that's blocked in msgrcv when the server
 *      removes the queue? (Check msgrcv's return value and errno: EIDRM.)
 *   3. Kill the server with Ctrl-C. Run `ipcs -q`. Is the queue still
 *      there? Add a SIGINT handler (Exercise 2 pattern) that sets a flag so
 *      the server removes the queue on the way out. Why can't you just call
 *      msgctl inside the handler? (Hmm — is msgctl async-signal-safe? Look
 *      it up; msgrcv returns -1/EINTR when interrupted, which helps.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex7_msgq_unrelated_processes.c -o ex7
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define KEY_FILE "ex7_msgq_unrelated_processes.c"
#define KEY_PROJ 'Q'
#define TEXT_LEN 128

struct request {
    long mtype;               /* always 1 */
    struct {
        pid_t client_pid;
        char text[TEXT_LEN];
    } body;
};

struct reply {
    long mtype;               /* = client pid */
    struct {
        char text[TEXT_LEN];
    } body;
};

int run_server( void );
int run_client( const char *text );
int run_flood( int n );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

int run_server( void ) { /* TODO */ return 0; }
int run_client( const char *text ) { /* TODO */ return 0; }
int run_flood( int n ) { /* TODO */ return 0; }
