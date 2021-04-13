/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2020
 *
 * Kernel Interrupt Service Routines
 */

#include "spede.h"
#include "kernel.h"
#include "kisr.h"
#include "kproc.h"
#include "ksyscall.h"
#include "syscall.h"
#include "queue.h"
#include "string.h"

/**
 * Kernel Interrupt Service Routine: Timer (IRQ 0)
 */
void kisr_timer() {

    int i, sleep_pid, check, temp, time;
    check = 0;
    sleep_pid = 0;
    temp = 0;
    time = 0;
    // Increment the system time
    system_time = system_time+1;
    /* !!! CODE NEEDED !!! */

    // If the running PID is invalid, just return
    // Since this is a hardware driven interrupt, make sure
    // to dismiss the IRQ

    /* !!! CODE NEEDED !!! */
    if (run_pid < 0 || run_pid > PID_MAX){
        outportb(0x20, 0x60);
        return;
        }

    // Increment the running process' current run time

    for(i=0; i < sizeof(&sleep_q); i++)
    {

         check = dequeue(&sleep_q, &sleep_pid);
         if(check != -1)
         {
             //breakpoint();
             temp = pcb[sleep_pid].wait_time;
             time = system_time;
             if(pcb[sleep_pid].wait_time < system_time)
             {
	         enqueue(&run_q, sleep_pid);
	     }
             else
             {
                 enqueue(&sleep_q, sleep_pid);
             }
         }
         else
	    break;
    }

    /* !!! CODE NEEDED !!! */
    pcb[run_pid].time = pcb[run_pid].time+1;

    // Once the running process has exceeded the maximum
    // number of ticks, it needs to be unscheduled:
    //   set the total run time
    //   reset the current running time
    //   set the state to ready
    //   queue the process back into the running queue
    //   clear the running pid

    /* !!! CODE NEEDED !!! */

    if (pcb[run_pid].time == PROC_TICKS_MAX){
        pcb[run_pid].total_time = pcb[run_pid].time + pcb[run_pid].total_time;
        pcb[run_pid].time = 0;
        pcb[run_pid].state = READY;

        if (run_pid == 0){
            enqueue(&idle_q, run_pid);
        }else{
            enqueue( &run_q, run_pid);
        }

        run_pid = -1;
    }
    // Dismiss IRQ 0 (Timer)
    outportb(0x20, 0x60);
}

void kisr_syscall() {

    int inter_switch;
    inter_switch = 0;
    inter_switch = pcb[run_pid].trapframe_p->eax;

   switch(inter_switch) {
       case SYSCALL_PROC_EXIT:
           kproc_exit();
           break;

       case SYSCALL_GET_SYS_TIME:
           ksyscall_get_sys_time();
           break;

       case SYSCALL_GET_PROC_PID:
           ksyscall_get_proc_pid();
           break;

       case SYSCALL_GET_PROC_NAME:
           ksyscall_get_proc_name();
           break;

       case SYSCALL_SLEEP:
           ksyscall_sleep();
           break;

       case SYSCALL_SEM_INIT:
           ksyscall_sem_init();
           break;

       case SYSCALL_SEM_WAIT:
           ksyscall_sem_wait();
           break;

       case SYSCALL_SEM_POST:
           ksyscall_sem_post();
           break;

       case SYSCALL_MSG_SEND:
           ksyscall_msg_send();
           break;

       case SYSCALL_MSG_RECV:
           ksyscall_msg_recv();
           break;
   }

   //outportb(0x80, 0x60);
}
