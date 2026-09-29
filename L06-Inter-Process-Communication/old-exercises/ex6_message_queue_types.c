/*
 * Lecture 6 — Exercise 6: Message Queues and Message Types
 *
 * GOAL
 *   Use msgget/msgsnd/msgrcv/msgctl between a parent and child with an
 *   IPC_PRIVATE queue, and master the three msgrcv type-selection modes.
 *
 * MESSAGE FORMAT
 *   struct job_msg { long mtype; struct { int job_id; char desc[32]; } job; };
 *   mtype is used as a PRIORITY: 1 = urgent, 2 = normal, 3 = low.
 *
 * PART A — FIFO (type == 0)
 *   - Create the queue: msgget(IPC_PRIVATE, 0600 | IPC_CREAT). Check for -1.
 *   - Fork.
 *   - Parent sends these 6 jobs in this order, then waits for the child:
 *       (3,"backup") (1,"fire alarm") (2,"email") (3,"defrag") (1,"power") (2,"print")
 *     Remember: the size argument is sizeof(msg.job), NOT sizeof(msg).
 *   - Child receives 6 messages with type = 0 and prints them in the order
 *     received.
 *   - Whoever finishes last removes the queue with msgctl(IPC_RMID).
 *
 * PART B — by exact type (type > 0)
 *   Child first drains all type-1 messages (use IPC_NOWAIT and stop when
 *   msgrcv fails with errno ENOMSG), then all type-2, then type-3.
 *   (Make the child sleep(1) first so all messages are already queued.)
 *
 * PART C — priority (type < 0)
 *   Child receives 6 messages with type = -3. What order do you get? Explain
 *   the rule from the lecture. How is this different from Part B?
 *
 * PART D — queue full
 *   In a separate mode, have the parent send messages in a loop with
 *   IPC_NOWAIT and NO receiver until msgsnd fails. Print how many fit and
 *   the errno name (EAGAIN). Compare to `cat /proc/sys/kernel/msgmnb`
 *   (max bytes per queue). Then remove the queue.
 *
 * DEBUGGING
 *   `ipcs -q` shows existing queues. If your program crashes before
 *   IPC_RMID, the queue LEAKS (it's owned by the OS, not your process).
 *   Clean up with `ipcrm -q <msqid>`.
 *
 * COMPILE / RUN
 *   gcc -Wall -Wextra -g ex6_message_queue_types.c -o ex6
 *   ./ex6 A     ./ex6 B     ./ex6 C     ./ex6 D
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

struct job_msg {
    long mtype;
    struct {
        int job_id;
        char desc[32];
    } job;
};

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}
