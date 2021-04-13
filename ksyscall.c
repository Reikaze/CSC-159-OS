/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2020
 *
 * System call APIs - Kernel Side
 */
#include "spede.h"
#include "kernel.h"
#include "kproc.h"
#include "string.h"
#include "queue.h"
#include "ksyscall.h"
//#include "ipc.h"

/**
 * System call kernel handler: get_sys_time
 * Returns the current system time (in seconds)
 */
void ksyscall_get_sys_time() {
    // Don't do anything if the running PID is invalid
    if(run_pid < 0 || run_pid > PID_MAX)
    {
        panic("Invalid PID");
    }
    // Copy the system time from the kernel to the
    // eax register via the running process' trapframe
    pcb[run_pid].trapframe_p->ebx= (system_time/CLK_TCK);
}

/**
 * System call kernel handler: get_proc_id
 * Returns the currently running process ID
 */
void ksyscall_get_proc_pid() {
    // Don't do anything if the running PID is invalid
    if (run_pid < 0 || run_pid > PID_MAX) {
        panic("Invalid PID");
    }

    // Copy the running pid from the kernel to the
    // eax register via the running process' trapframe
    pcb[run_pid].trapframe_p->ebx = run_pid;
}

/**
 * System call kernel handler: get_proc_name
 * Returns the currently running process name
 */
void ksyscall_get_proc_name() {
    //pointer for setting the destionation Pointer
    char *point; 
    // Don't do anything if the running PID is invalid
    if(run_pid < 0 || run_pid > PID_MAX) {
        panic("Invalid PID");
    }
    // Set the destination pointer to the address passed in via EBX
    // Copy the string name from the PCB to the destination
    point =  pcb[run_pid].trapframe_p->ebx;
    //breakpoint();
    sp_memcpy( (void *)point, (void *)&pcb[run_pid].name, sizeof(pcb[run_pid].name) );
}

/**
 * System call kernel handler: sleep
 * Puts the currently running process to sleep
 */
void ksyscall_sleep() {

    // Varaiable used for calculating wake time
    int sleepTime;
    sleepTime =0;
    // Don't do anything if the running PID is invalid
    if (run_pid < 0 || run_pid > PID_MAX) {
        panic("Invalid PID");
    }
    // Calculate the wake time for the currently running process
    sleepTime = 2*run_pid;
    sleepTime = sleepTime%10;
    sleepTime++;

    sleepTime = sleepTime*CLK_TCK;

    // Store this time in the pcb's wake_time member
    pcb[run_pid].wait_time=sleepTime+system_time; 

    // Move the currently running process to the sleep queue
    enqueue(&sleep_q, run_pid);

    // Change the running process state to SLEEP
    pcb[run_pid].state = SLEEPING;

    // Clear the running PID so the process scheduler will run
    run_pid = -1;
}

void ksyscall_sem_init(){
    int i, sem_id;
    int *sem;
    i=0;
    sem_id=-1;
    sem =(int *)pcb[run_pid].trapframe_p->ebx;

    // We are checking to see if the semaphore is uninitialized or not
    // if not initialized we will go into the if statement to initialize it
    if( *sem == SEMAPHORE_UNINITIALIZED)
    {
        //if not initialized we dequeue from the semaphore queue and get a semaphore id
        dequeue(&semaphore_q, &sem_id);
        // the id passed in is changed to this new proper sem id
        *sem= sem_id;
        //We set the semaphore count to zero
        semaphores[sem_id].count=0;
        //We make set the semaphore to Initialized 
        semaphores[sem_id].init=SEMAPHORE_INITIALIZED;
    }
    //semaphore is initialized
}

void ksyscall_sem_wait(){

    int *sem_id;

    sem_id=(int *)pcb[run_pid].trapframe_p->ebx;

    //Check to see if the semaphore pid is valid
    if(*sem_id < 0 || *sem_id > SEMAPHORE_MAX) {
        panic("Invalid Semaphore PID");
    }

    //We are checking if the semaphore has any other process waiting
    if(semaphores[*sem_id].count > 0){

        //We are putting the current runing process to wait
        enqueue(&semaphores[*sem_id].wait_q, run_pid);

        //We change the state of the process to waiting
        pcb[run_pid].state = WAITING;

        //Clear the running pid to trigger process schedular
        run_pid=-1;
    }

    // increment the amount of processes waiting
    semaphores[*sem_id].count = semaphores[*sem_id].count +1;

}

void ksyscall_sem_post(){
    int temp;
    int *sem_id;
    temp = -1;
    sem_id=(int *)pcb[run_pid].trapframe_p->ebx;

    //Check to see if the semaphore pid is valid
    if(*sem_id < 0 || *sem_id > SEMAPHORE_MAX) {
        panic("Invalid Semaphore PID");
    }

    //We are checking to see if there are any process waiting to be released
    if(semaphores[*sem_id].count > 0){
        //We get the first waiting process id that was waiting for the semaphore
        dequeue(&semaphores[*sem_id].wait_q, &temp);

        //We change the waiting process status to ready
        pcb[temp].state=READY;

        //We move the waiting process to the run queue
        enqueue(&run_q, temp);

        // decrease the number of waiting semaphores
        semaphores[*sem_id].count = semaphores[*sem_id].count-1;

    }
}

void ksyscall_msg_send(){

    msg_t *msg, *msg_recv;
    int mailbox, check, empty, recv_pid;
    check = -1;
    empty = -1;
    recv_pid = -1;
    msg = pcb[run_pid].trapframe_p->ebx;
    mailbox = pcb[run_pid].trapframe_p->ecx;

    // Set Sender PID & the time the message is sent
    msg->sender = run_pid;
    msg->time_sent = system_time/CLK_TCK;

    // We are checking to see if message queue is full or not
    check = mbox_enqueue(msg, mailbox);
    if(check == -1){
        panic("Message queue is full");
    }

    // We are checking is the mailbox has other process waiting for a message in the mailboxs wait queue
    empty = dequeue(&mailboxes[mailbox].wait_q, &recv_pid);
    if(empty != -1){
        pcb[recv_pid].state = READY;
        enqueue(&run_q, recv_pid);
        msg_recv = pcb[recv_pid].trapframe_p->ebx;
        mbox_dequeue(msg_recv, mailbox);
        msg_recv->time_received = system_time/CLK_TCK;
    }
}

void ksyscall_msg_recv(){

    msg_t *msg;
    int mailbox, check, empty;
    check = -1;
    empty = -1;

    // We are collecting the mailbox id from the kernel registers
    mailbox=pcb[run_pid].trapframe_p->ecx;

    // If we have a message in the mailbox then We will get the message from the mailbox
    if(mailboxes[mailbox].size > 0){

        // We are collecting the message pointer from the kernel registers
        msg=pcb[run_pid].trapframe_p->ebx;
        // We are collecting the message from the mailbox
        check = mbox_dequeue(msg, mailbox);
        // if we get an error in dequeueing we will trigger a panic
        if(check == -1){
            panic("Message could not be dequeued an Error Ocurred");
       msg->time_received = system_time/CLK_TCK; 
       }
    }
    // Else we will put the running process will be put the running process to the waiting queue
    else{
        // We are changing the state of the running process
        pcb[run_pid].state=WAITING;
        // We are moving the process to the mailbox wait queue to wait for a message
        enqueue(&mailboxes[mailbox].wait_q, run_pid);
        // We clear the running pid to trigger the schedular
        run_pid = -1;
    }
}

int mbox_enqueue(msg_t *msg, int mbox_num){


    if(mailboxes[mbox_num].size > MBOX_SIZE){
       // panic("Message queue is full");
       return -1;
    }
    else{
        mailboxes[mbox_num].messages[mailboxes[mbox_num].tail]=*msg;
        mailboxes[mbox_num].tail= mailboxes[mbox_num].tail+1;
        mailboxes[mbox_num].size= mailboxes[mbox_num].size+1;
        if(mailboxes[mbox_num].tail > MBOX_SIZE){
            mailboxes[mbox_num].tail=0;
        }
        return 0;
    }
}


int mbox_dequeue(msg_t *msg, int mbox_num){

    if(mailboxes[mbox_num].size == 0){
        //panic("Message queue is empty");
        return -1;
    }
    else{
       /* // Since the message is getting dequeued then that means a process is receving the message & we are setting the time here
        mailboxes[mbox_num].messages[mailboxes[mbox_num].head].time_received = system_time/CLK_TCK;*/
        // We are setting the message pointer to the message saved in the message queue
        *msg = mailboxes[mbox_num].messages[mailboxes[mbox_num].head];
        // We increment the head of the queue & decrease the size of the queue
        mailboxes[mbox_num].head= mailboxes[mbox_num].head+1;
        mailboxes[mbox_num].size= mailboxes[mbox_num].size-1;

        // If the head reaches the end of the queue size we want to loop back around
        if(mailboxes[mbox_num].head == MBOX_SIZE)
        {
            mailboxes[mbox_num].head=0;
        }
        return 0;
    }
}
