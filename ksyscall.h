/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2020
 *
 * System call APIs - Kernel Side
 */
#ifndef KSYSCALL_H
#define KSYSCALL_H

/* System information */
void ksyscall_get_sys_time();

/* Process information */
void ksyscall_get_proc_pid();
void ksyscall_get_proc_name();

/* Additional functionality */
void ksyscall_sleep();
void ksyscall_sem_init();
void ksyscall_sem_wait();
void ksyscall_sem_post();
void ksyscall_msg_send();
void ksyscall_msg_recv();

//helper functions
int mbox_enqueue(msg_t *msg, int mbox_num);
int mbox_dequeue(msg_t *msg, int mbox_num);

#endif
