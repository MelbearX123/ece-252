/*
 * Lecture 6 — Exercise 1: IPC Through the File System
 *
 * GOAL
 *   Implement the lecture's "import directory" pattern: a producer drops
 *   uniquely named files into a directory, a consumer scans for them,
 *   processes and deletes them. No overwrites possible.
 *
 * SPEC
 *   ./ex1 <num_messages>
 *   - mkdir("inbox", 0700) (ignore EEXIST).
 *   - Fork.
 *   - PRODUCER (parent): for i in 0..num_messages-1:
 *       * build the message "order <i>: <random 1-100> widgets"
 *       * write it to "inbox/msg_<i>.tmp" with fopen/fprintf/fclose
 *       * THEN rename("inbox/msg_<i>.tmp", "inbox/msg_<i>.txt")
 *       * usleep a random 0-200 ms
 *     Finally write "inbox/msg_<num_messages>.txt" containing "DONE",
 *     then wait() for the consumer.
 *   - CONSUMER (child): loop:
 *       * try to open "inbox/msg_<next>.txt"
 *       * if it doesn't exist yet, usleep(50 ms) and retry (this is POLLING —
 *         Lecture 2! note the cost)
 *       * otherwise read it, print "[consumer] got: <text>", remove() it,
 *         next++. Stop after reading "DONE".
 *   - At the end, rmdir("inbox").
 *
 * WHY THE .tmp + rename?
 *   Without it, the consumer might open msg_3.txt after the producer has
 *   CREATED it but before it has finished WRITING it and see an empty or
 *   partial message. rename() within a directory is atomic: the file
 *   appears under its final name all at once. Try removing the rename
 *   trick and adding a sleep between fopen and fprintf in the producer —
 *   can you get the consumer to read an empty message?
 *
 * QUESTIONS
 *   1. Which sync/async combination is this (send? receive?)
 *   2. Kill the consumer halfway (add a crash at message 5). Re-run only a
 *      consumer later — are the unread messages still there? Which IPC
 *      property from the lecture is this?
 *   3. What's wasteful about the consumer's loop? How do message queues
 *      (Ex 6) avoid it?
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex1_file_ipc.c -o ex1
 */

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DIR_NAME "inbox"

void producer( int n );
void consumer( void );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

void producer( int n ) { /* TODO */ }
void consumer( void ) { /* TODO */ }
