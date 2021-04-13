/**
 * CPE/CSC 159 - Operating System Pragmatics
 * California State University, Sacramento
 * Fall 2020
 *
 * Internal Kernel APIs
 */
#include "spede.h"

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a panic message on the target console
 *   - Triggers a breakpiont (if running through GDB)
 *   - aborts/exits
 * @param msg   the message to display
 */
void panic(char *msg) {
    // Display a message indicating a panic was hit
    // trigger a breakpoint
    // abort since this is a fatal condition!
    printf("%s\n", msg);
    breakpoint();
    exit(1);
    /* !!! CODE NEEDED !!! */
}

/**
 * Triggers a kernel panic that does the following:
 *   - Displays a warning message on the target console
 *   - Triggers a breakpoint (if running through GDB)
 * @param msg   the message to display
 */
void panic_warn(char *msg) {
    // Display a message indicating a warning was hit
    // trigger a breakpoint
    printf("%s\n", msg);
    breakpoint();
    /* !!! CODE NEEDED !!! */
}

/**
 * Prints to the host console if the DEBUG flag is set
 * @param format    string format to be used for printing
 * @param ...       variable arguments for format string
 */
void debug_printf(const char *format, ...) {
#ifdef DEBUG
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#else
    return;
#endif
}
