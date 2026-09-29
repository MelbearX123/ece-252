/*
 * Lecture 4 — Exercise 2: A Toy PCB and Five-State Machine
 *
 * GOAL
 *   Model the OS's view of processes in C: a PCB struct, a list of PCBs,
 *   and a transition function that only allows legal five-state moves.
 *   (Also good practice of Lecture 1: structs, enums, heap, pointers.)
 *
 * TASKS
 *   1. Complete pcb_t with: pid, state, priority, program_counter,
 *      registers[NUM_REGS], and cpu_time_used (accounting info).
 *   2. pcb_create(): malloc a PCB, give it the next pid (a static counter
 *      starting at 1), state NEW, zero the registers with memset. The
 *      "Create" transition.
 *   3. transition(pcb, t): apply transition t if it's legal from the
 *      current state; return 0 on success, -1 (and leave state unchanged)
 *      if illegal. Legal moves:
 *         ADMIT    NEW -> READY
 *         DISPATCH READY -> RUNNING
 *         SUSPEND  RUNNING -> READY
 *         BLOCK    RUNNING -> BLOCKED
 *         UNBLOCK  BLOCKED -> READY
 *         EXIT     RUNNING -> TERMINATED
 *         KILL     READY|BLOCKED|RUNNING -> TERMINATED   (the "not shown" exits)
 *      REAP is special — see pcb_reap.
 *   4. context_switch(from, to, cpu): simulate saving the CPU's pc and
 *      registers into `from` (SUSPEND it), then loading them from `to`
 *      (DISPATCH it). cpu_t is provided. Only one process may be RUNNING.
 *   5. pcb_reap(): only valid on TERMINATED; free the PCB. Return -1 if
 *      the process is not terminated (you can't reap a live process!).
 *   6. main: run the scripted scenario below, printing each process's state
 *      after every step with print_pcb(). Also try 3 ILLEGAL transitions and
 *      confirm they're rejected.
 *
 * SCENARIO
 *   create P1, P2
 *   admit P1, admit P2
 *   dispatch P1                (cpu.pc = 100 for P1's work; set cpu regs to some values)
 *   P1 blocks on I/O
 *   dispatch P2
 *   P1 unblocks
 *   context_switch(P2 -> P1)   (P1's saved pc/regs must come back exactly!)
 *   P1 exits
 *   reap P1
 *   kill P2 (while READY)
 *   reap P2
 *   (valgrind should report no leaks)
 *
 * STRETCH
 *   Keep all PCBs in a linked list (add a `next` pointer) like the kernel's
 *   container, and implement a simple round-robin "scheduler" that
 *   dispatches the next READY process each tick.
 *
 * COMPILE
 *   gcc -Wall -Wextra -g ex2_pcb_state_machine.c -o ex2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_REGS 4

typedef enum { NEW, READY, RUNNING, BLOCKED, TERMINATED } state_t;
typedef enum { ADMIT, DISPATCH, SUSPEND, BLOCK, UNBLOCK, EXIT, KILL } transition_t;

static const char *STATE_NAMES[] = { "New", "Ready", "Running", "Blocked", "Terminated" };
static const char *TRANSITION_NAMES[] = { "Admit", "Dispatch", "Suspend", "Block",
                                          "Unblock", "Exit", "Kill" };

typedef struct {
    unsigned long pc;
    long regs[NUM_REGS];
} cpu_t;

typedef struct pcb {
    /* TODO 1 */
    state_t state;
} pcb_t;

pcb_t *pcb_create( int priority );
int transition( pcb_t *p, transition_t t );
int context_switch( pcb_t *from, pcb_t *to, cpu_t *cpu );
int pcb_reap( pcb_t *p );
void print_pcb( const pcb_t *p );

int main( void ) {
    /* TODO 6 */
    (void) STATE_NAMES; (void) TRANSITION_NAMES;
    return 0;
}

pcb_t *pcb_create( int priority ) { /* TODO 2 */ return NULL; }
int transition( pcb_t *p, transition_t t ) { /* TODO 3 */ return -1; }
int context_switch( pcb_t *from, pcb_t *to, cpu_t *cpu ) { /* TODO 4 */ return -1; }
int pcb_reap( pcb_t *p ) { /* TODO 5 */ return -1; }
void print_pcb( const pcb_t *p ) { /* TODO */ }
