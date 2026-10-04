/**
 * Function Tracing Helper - FTrace Integration
 *
 * Provides utilities for kernel function tracing and hooking via ftrace.
 * Requires CONFIG_FTRACE and CONFIG_HAVE_DYNAMIC_FTRACE in kernel.
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* FTrace hook registration */
typedef struct {
    const char* function_name;
    void (*pre_handler)(void);
    void (*post_handler)(void);
} ftrace_hook_t;

/* Register ftrace hook for a kernel function */
int jocky_ftrace_hook_register(ftrace_hook_t* hook);

/* Unregister ftrace hook */
int jocky_ftrace_hook_unregister(ftrace_hook_t* hook);

/* Enable ftrace tracing */
int jocky_ftrace_enable(void);

/* Disable ftrace tracing */
int jocky_ftrace_disable(void);

/* Get ftrace tracing status */
int jocky_ftrace_is_enabled(void);

#ifdef __cplusplus
}
#endif
