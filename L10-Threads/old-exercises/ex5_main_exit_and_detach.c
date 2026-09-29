/*
 * Lecture 10 — Exercise 5: How main Ends, and Detached Threads
 *
 * Every thread here runs slow_worker(): it prints "[worker k] step s" once
 * per second for 3 seconds, then "[worker k] finished".
 *
 * MODE return   ./ex5 return
 *   main creates 3 workers, prints "main returning", and returns 0
 *   WITHOUT joining. What do you see? Why? (Lecture: main returning ends
 *   the process, and all its threads are suddenly terminated.)
 *
 * MODE exit     ./ex5 exit
 *   Same, but call exit(0) from main. Same result? Now instead have a
 *   WORKER call exit(0) after its first step. What happens to the others
 *   and to main?
 *
 * MODE pexit    ./ex5 pexit
 *   main creates 3 workers, prints "main calling pthread_exit", then
 *   pthread_exit(NULL). Do the workers finish? When does the process end?
 *   Check with `ps -L -p <pid>` (the -L shows threads — "LWP") in another
 *   terminal while it runs: how many threads exist before/after main's
 *   pthread_exit?
 *
 * MODE detach   ./ex5 detach
 *   Create workers DETACHED, two ways:
 *     worker 0: pthread_attr_init + pthread_attr_setdetachstate(&attr,
 *               PTHREAD_CREATE_DETACHED), pass &attr to pthread_create,
 *               then pthread_attr_destroy(&attr).
 *     worker 1: create normally, then call pthread_detach(t1).
 *   Then try pthread_join on each — print the return code with strerror.
 *   (Lecture: joining a detached thread is a logical error; expect EINVAL.
 *    Note: joining after a detached thread has already exited is undefined
 *    behaviour, so join right away.)
 *   How should main wait for detached workers, if not with join? Use
 *   pthread_exit in main for now.
 *
 * MODE crash    ./ex5 crash
 *   Worker 2 dereferences NULL at step 2. Do workers 0 and 1 survive?
 *   Compare with Lecture 9 Exercise 5 where children were PROCESSES — if
 *   one child segfaulted, did the others survive? (Lecture drawback #2.)
 *
 * COMPILE
 *   gcc -Wall -Wextra -g -pthread ex5_main_exit_and_detach.c -o ex5
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int id;
    int crash_at_step;      /* -1 = never */
    int exit_at_step;       /* -1 = never */
} worker_args_t;

void *slow_worker( void *arg );

int main( int argc, char **argv ) {
    /* TODO */
    return 0;
}

void *slow_worker( void *arg ) { /* TODO */ return NULL; }
