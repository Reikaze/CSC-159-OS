/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2020
 *
 * Internal Kernel APIs and Data structures
 */

#ifndef KERNEL_H
#define KERNEL_H

#include "global.h"
#include "queue.h"
#include "trapframe.h"
#include "ipc.h"

// Global Definitions

// Maximum process ID possible (0-based PIDs)
#define PID_MAX PROC_MAX-1

// Process runtime stack size
#define PROC_STACK_SIZE 8196

// Maximum number of ticks a process may run before being rescheduled
#define PROC_TICKS_MAX 50

// Maximum number of Semaphores that we can create
#define SEMAPHORE_MAX PROC_MAX

//Maximum number of Mailboxes
#define MBOX_MAX PROC_MAX

//Maximum size  of Mailboxes
#define MBOX_SIZE PROC_MAX


/**
 * Kernel data types and definitions
 */

// Process states
typedef enum {
    AVAILABLE,
    READY,
    RUNNING,
    SLEEPING,
    WAITING
} state_t;

// The process control block for each process
typedef struct {
    char name[PROC_NAME_LEN+1];     // Process name/title
    state_t state;                  // current process state
    queue_t *queue;                 // queue the process belongs to
    int time;                       // run time since loaded
    int total_time;                 // total run time since created
    int wait_time;         			// The time when a process is done sleeping
    trapframe_t *trapframe_p;       // process trapframe
} pcb_t;


//Semaphore data structure
typedef struct{
    int count;                      //Semaphore Count
    int init;                       // Indicates if Initialized
    queue_t wait_q;                 // Wait queue for Semaphore
} semaphore_t;

//Mailbox Data Structure
typedef struct{
    msg_t messages[MBOX_SIZE];      // Incoming messages
    int head;                       // First message
    int tail;                       // Last message
    int size;                       // Total messages
    queue_t wait_q;                 // Processes waiting for messages
} mailbox_t;


/**
 * Kernel data structures - available to the entire kernel
 */

// runtime stacks of processes
extern char stack[PROC_MAX][PROC_STACK_SIZE];

// process table
extern pcb_t pcb[PROC_MAX];

// System time
extern int system_time;

// ID of running process, -1 means not set
extern int run_pid;

// Process queues
extern queue_t available_q;
extern queue_t run_q;
extern queue_t idle_q;
extern queue_t sleep_q;

//Semaphore Data Structure
extern semaphore_t semaphores[SEMAPHORE_MAX];
extern queue_t semaphore_q;

//Mailbox Data Structure
extern mailbox_t mailboxes[MBOX_MAX];

/**
 * Function declarations
 */

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a panic message
 *   - Triggers a breakpiont (if running through GDB)
 *   - aborts/exits
 * @param msg   message to be displayed
 */
void panic(char *msg);

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a warning message
 *   - Triggers a breakpoint (if running through GDB)
 * @param msg   message to be displayed
 */
void panic_warn(char *msg);


/**
 * Prints to host console if DEBUG flag is enabled
 * @param format    string format to be used for printing
 * @param ...       variable arguments for format string
 */
void debug_printf(char *format, ...);

#endif
