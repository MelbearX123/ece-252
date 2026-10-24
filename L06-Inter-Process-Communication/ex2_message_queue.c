/*
 * Lecture 6 — Exercise 2: A Message Queue Between Parent and Child
 *
 * GOAL
 *   Use msgget / msgsnd / msgrcv / msgctl.
 *
 * SPEC
 *   - Create a queue:  msgget( IPC_PRIVATE, IPC_CREAT | 0600 )
 *   - Fork.
 *   - Child: fill in a struct msg with mtype = 1 and text "hello from child",
 *     msgsnd it, and exit(0).
 *   - Parent: msgrcv a message of type 1, print
 *       "parent got: <text>"
 *     then wait(NULL) and remove the queue with msgctl( id, IPC_RMID, NULL ).
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex2_message_queue.c -o ex2
 *   ./ex2
 *   ipcs -q       (the queue should be gone afterwards)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct msg {
    long mtype;
    char text[64];
};

int main( void ) {
    /* TODO */
    int msgqid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
    pid_t pid = fork();
    if (pid > 0) {
        struct msg m2;
        msgrcv(msgqid, &m2, sizeof(m2.text), 1, 0);
        printf("parent got: %s\n", m2.text);
        wait(NULL);
        msgctl(msgqid, IPC_RMID, NULL);

    } else if (pid == 0) {
        struct msg m;
        m.mtype = 1;
        strcpy( m.text, "hello from child" );
        msgsnd(msgqid, &m, sizeof(m.text), 0);
        exit(0);
    } else {
        return 1;
    }
    return 0;
}
