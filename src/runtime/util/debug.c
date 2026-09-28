#include "jocky_debug.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

/* Global debugger instance */
JockyDebugger jocky_debugger = {0};

/* Debug callback */
static JockyDebugCallback debug_callback = NULL;

void jocky_set_debug_callback(JockyDebugCallback callback) {
    debug_callback = callback;
}

void jocky_set_breakpoint(uintptr_t addr, const char *location) {
    if (jocky_debugger.num_breakpoints >= JOCKY_MAX_BREAKPOINTS) {
        fprintf(stderr, "JOCKY: Maximum breakpoints reached\n");
        return;
    }

    /* Check if already set */
    for (int i = 0; i < jocky_debugger.num_breakpoints; i++) {
        if (jocky_debugger.breakpoints[i].address == addr) {
            return;
        }
    }

    JockyBreakpoint *bp = &jocky_debugger.breakpoints[jocky_debugger.num_breakpoints++];
    bp->address = addr;
    bp->location = location;
    bp->enabled = true;

    /* Save original byte, replace with INT3 (0xCC on x86) */
    uint8_t *code = (uint8_t *)addr;

    /* Make page writable */
    uintptr_t page_addr = addr & ~(getpagesize() - 1);
    mprotect((void *)page_addr, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC);

    bp->original_byte = *code;
    *code = 0xCC;  /* INT3 breakpoint instruction */

    /* Restore protection */
    mprotect((void *)page_addr, getpagesize(), PROT_READ | PROT_EXEC);
}

void jocky_remove_breakpoint(uintptr_t addr) {
    for (int i = 0; i < jocky_debugger.num_breakpoints; i++) {
        if (jocky_debugger.breakpoints[i].address == addr) {
            JockyBreakpoint *bp = &jocky_debugger.breakpoints[i];

            /* Make page writable */
            uintptr_t page_addr = addr & ~(getpagesize() - 1);
            mprotect((void *)page_addr, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC);

            /* Restore original byte */
            uint8_t *code = (uint8_t *)addr;
            *code = bp->original_byte;

            /* Restore protection */
            mprotect((void *)page_addr, getpagesize(), PROT_READ | PROT_EXEC);

            bp->enabled = false;

            /* Remove from array by shifting */
            if (i < jocky_debugger.num_breakpoints - 1) {
                memmove(&jocky_debugger.breakpoints[i],
                        &jocky_debugger.breakpoints[i + 1],
                        (jocky_debugger.num_breakpoints - i - 1) * sizeof(JockyBreakpoint));
            }
            jocky_debugger.num_breakpoints--;
            break;
        }
    }
}

void jocky_enable_breakpoint(uintptr_t addr, bool enabled) {
    for (int i = 0; i < jocky_debugger.num_breakpoints; i++) {
        if (jocky_debugger.breakpoints[i].address == addr) {
            jocky_debugger.breakpoints[i].enabled = enabled;
            break;
        }
    }
}

JockyBreakpoint *jocky_get_breakpoint(uintptr_t addr) {
    for (int i = 0; i < jocky_debugger.num_breakpoints; i++) {
        if (jocky_debugger.breakpoints[i].address == addr) {
            return &jocky_debugger.breakpoints[i];
        }
    }
    return NULL;
}

static void jocky_breakpoint_handler(int sig, void *addr) {
    if (sig == SIGTRAP) {
        JockyBreakpoint *bp = jocky_get_breakpoint((uintptr_t)addr);
        if (bp && bp->enabled && debug_callback) {
            debug_callback("breakpoint", (uintptr_t)addr);
        }
    }
}

void jocky_handle_breakpoint_signal(int sig) {
    /* Signal handler - in real implementation, would be connected via sigaction */
    if (sig == SIGTRAP) {
        fprintf(stderr, "JOCKY: Breakpoint hit\n");
    }
}

void jocky_debug_init(void) {
    memset(&jocky_debugger, 0, sizeof(jocky_debugger));

    /* Install signal handlers for SIGTRAP */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = (void (*)(int))jocky_handle_breakpoint_signal;
    sigaction(SIGTRAP, &sa, NULL);

    fprintf(stderr, "JOCKY: Debugger initialized\n");
}

void jocky_debug_cleanup(void) {
    /* Remove all breakpoints */
    while (jocky_debugger.num_breakpoints > 0) {
        jocky_remove_breakpoint(jocky_debugger.breakpoints[0].address);
    }
    fprintf(stderr, "JOCKY: Debugger cleaned up\n");
}
