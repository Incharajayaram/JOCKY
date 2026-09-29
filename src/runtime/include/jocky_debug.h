#ifndef JOCKY_DEBUG_H
#define JOCKY_DEBUG_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JOCKY_MAX_BREAKPOINTS 256

typedef struct {
    uintptr_t address;
    uint8_t original_byte;
    bool enabled;
    const char *location;  /* file:line format */
} JockyBreakpoint;

typedef struct {
    JockyBreakpoint breakpoints[JOCKY_MAX_BREAKPOINTS];
    int num_breakpoints;
} JockyDebugger;

/* Global debugger instance */
extern JockyDebugger jocky_debugger;

/* Set breakpoint at address */
void jocky_set_breakpoint(uintptr_t addr, const char *location);

/* Remove breakpoint at address */
void jocky_remove_breakpoint(uintptr_t addr);

/* Enable/disable breakpoint */
void jocky_enable_breakpoint(uintptr_t addr, bool enabled);

/* Get breakpoint info */
JockyBreakpoint *jocky_get_breakpoint(uintptr_t addr);

/* Breakpoint signal handler */
void jocky_handle_breakpoint_signal(int sig);

/* Debug event callback (called when debugger stops) */
typedef void (*JockyDebugCallback)(const char *event, uintptr_t addr);
void jocky_set_debug_callback(JockyDebugCallback callback);

/* Initialize debugger */
void jocky_debug_init(void);

/* Cleanup debugger */
void jocky_debug_cleanup(void);

#ifdef __cplusplus
}
#endif

#endif /* JOCKY_DEBUG_H */
