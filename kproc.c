/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2020
 *
 * Kernel Process Handling
 */
#include "spede.h"
#include "kernel.h"
#include "kproc.h"
#include "queue.h"
#include "string.h"

/**
 * Process scheduler
 */
void kproc_schedule() {
    int qNum;
    int check;
    // The time when a process is done sleeping


    // if we already have an active process that is running, we should simply return
    //
   /* if (run_pid > 0 || run_pid < PID_MAX)
        return ;*/
    // dequeue a process from the running queue
    // if it is a valid process
    //   Set the active running PID
    //   Set the process state to RUNNING
    //
    if(pcb[run_pid].state == RUNNING)
        return ;


    qNum = 0;
    check = 0;
    check = dequeue(&run_q, &qNum);


    if(check == -1)
    {
        check = dequeue(&idle_q, &qNum);
    }



   // breakpoint();
    run_pid = qNum;

    pcb[run_pid].state = RUNNING;
    // if a process cannot be dequeued, trigger a panic
    //
    if (check == -1)
      panic("process cannot be dequeued");
    // if we don't have a valid running process, trigger a panic
    if (run_pid < 0 || run_pid > PROC_MAX)
        panic("not a valid running process");
    /* !!! CODE NEEDED !!! */
    debug_printf("Scheduled process %s (pid=%d)\n", pcb[run_pid].name, run_pid);
}

/**
 * Start a new process
 * @param proc_name The process title
 * @param proc_ptr  function pointer for the process
 * @param queue     the run queue in which this process belongs
 */
void kproc_exec(char *proc_name, void *proc_ptr, queue_t *queue) {

    int pid; //&item;//item is psudocode 

    int qNum, queueNum;

    pid=0;
    qNum=0;
    queueNum=0;
    // Ensure that valid parameters have been specified

    /* !!! CODE NEEDED !!! */
    if (proc_name == NULL || proc_ptr  == NULL|| queue == NULL){
        proc_name = "ktask_idle";
        proc_ptr = &ktask_idle;
        proc_ptr = &run_q;
    }
    // Dequeue the process from the available queue
    // If a process cannot be dequeued, trigger a warning
    /* !!! CODE NEEDED !!! */

    //breakpoint();
    qNum = dequeue( &available_q, &pid);

    if (qNum == -1)
        panic("process cannot be dequeued");
    // Initialize the PCB
    //   Set the process state to READY
    //   Initialize other process control block variables to default values
    //   Copy the process name to the PCB
    //   Ensure the stack for the process is initialized

    /* !!! CODE NEEDED !!! */
   /* *(pcb_t *)(proc_ptr).state = READY;
    *(pcb_t *)(proc_ptr).time = 0;
    *(pcb_t *)(proct_ptr).total_time = 0;
   */
    pcb[pid].state = READY;
    pcb[pid].time = 0;
    pcb[pid].total_time = 0;

    pcb[pid].wait_time = 0;

    //pcb[pid].name = &proc_name;
    sp_strcpy((char *) pcb[pid].name , proc_name );

    sp_memset(&stack[pid], 0, sizeof(stack[pid]));

    // Allocate the trapframe data
    pcb[pid].trapframe_p = (trapframe_t *)&stack[pid][PROC_STACK_SIZE - sizeof(trapframe_t)];

    // Set the instruction pointer in the trapframe
    pcb[pid].trapframe_p->eip = (unsigned int)proc_ptr;

    // Set INTR flag
    pcb[pid].trapframe_p->eflags = EF_DEFAULT_VALUE | EF_INTR;

    // Set each segment in the trapframe
    pcb[pid].trapframe_p->cs = get_cs();
    pcb[pid].trapframe_p->ds = get_ds();
    pcb[pid].trapframe_p->es = get_es();
    pcb[pid].trapframe_p->fs = get_fs();
    pcb[pid].trapframe_p->gs = get_gs();


    // Set the process run queue (supplied as argument)
    // Move the proces into the associated run queue
    /* !!! CODE NEEDED !!! */
    //pcb[pid].queue = *queue;


    queueNum = enqueue( queue, pid);

    //breakpoint();

    sp_memcpy((void *)&pcb[pid].queue, (void *)queue, sizeof(queue));

    debug_printf("Started process %s (pid=%d)\n", pcb[pid].name, pid);
}

/**
 * Exit the currently running process
 */
void kproc_exit() {
    // PID 0 should be our kernel idle task.
    // It should never exit.
    /* !!! CODE NEEDED !!! */
    int queueNum;
    if (run_pid == 0)
        return;
    // Panic if we have an invalid PID
    /* !!! CODE NEEDED !!! */
    if (run_pid < 0 || run_pid > 255)
        panic("invalid PID");
    debug_printf("Exiting process %s (pid=%d)\n", pcb[run_pid].name, run_pid);

    // Change the state of the running process to AVAILABLE
    // Queue it back to the available queue
    // clear the running pid
    /* !!! CODE NEEDED !!! */
    pcb[run_pid].state = AVAILABLE;
    queueNum = enqueue(&available_q, run_pid);
    run_pid = 0;
    // Trigger the scheduler to load the next process
    /* !!! CODE NEEDED !!! */
    kproc_schedule();
}


/**
 * Kernel idle task
 */
void ktask_idle() {
    int i;

    // Indicate that the Idle Task has started
    cons_printf("idle_task started\n");

    // Process run loop
    while (1) {
        asm("hlt;");
    }
}
