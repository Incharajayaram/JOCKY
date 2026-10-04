/**
 * Function Tracing Helper - FTrace Integration (Portable Implementation)
 *
 * Portable implementation that works without kernel headers.
 * For production use with actual kernel module, compile with kernel headers.
 */

#include "ftrace_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int ftrace_enabled = 0;

int jocky_ftrace_hook_register(ftrace_hook_t* hook) {
    if (!hook || !hook->function_name) {
        return -1;
    }
    return 0;
}

int jocky_ftrace_hook_unregister(ftrace_hook_t* hook) {
    if (!hook) {
        return -1;
    }
    return 0;
}

int jocky_ftrace_enable(void) {
    ftrace_enabled = 1;
    return 0;
}

int jocky_ftrace_disable(void) {
    ftrace_enabled = 0;
    return 0;
}

int jocky_ftrace_is_enabled(void) {
    return ftrace_enabled;
}
