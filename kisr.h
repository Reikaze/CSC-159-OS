/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2020
 *
 * Kernel Interrupt Service Routines
 */
#ifndef KISR_H
#define KISR_H

/**
 * Interrupt Service Routine definitions
 */

// Interrupt definitions
#define TIMER_INTR 0x20
#define SYS_INTR   0x80

// kernel's stack size in bytes
#define KSTACK_SIZE 16384

// kernel's code segment
#define KCODE 0x08

// kernel's data segment
#define KDATA 0x10


#ifndef ASSEMBLER
/**
 * Function declarations
 */

// Timer ISR
void kisr_timer();

// Syscall ISR
void kisr_syscall();

/* Defined in kisr_entry.S */
__BEGIN_DECLS

// Kernel interrupt entries
extern void kisr_entry_timer();
extern void kisr_entry_syscall();

__END_DECLS
#endif
#endif
