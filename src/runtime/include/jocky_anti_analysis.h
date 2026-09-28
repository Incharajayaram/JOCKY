#ifndef JOCKY_ANTI_ANALYSIS_H
#define JOCKY_ANTI_ANALYSIS_H

#include <stdint.h>

typedef void* jocky_anti_analysis_ctx_t;

/* Detection methods */
#define JOCKY_DETECT_DEBUGGER 1
#define JOCKY_DETECT_IDA 2
#define JOCKY_DETECT_VALGRIND 3
#define JOCKY_DETECT_STRACE 4
#define JOCKY_DETECT_GDBSERVER 5
#define JOCKY_DETECT_PTRACE 6

/* Evasion techniques */
#define JOCKY_EVADE_BREAKPOINT 1
#define JOCKY_EVADE_SINGLE_STEP 2
#define JOCKY_EVADE_CODE_CAVE 3
#define JOCKY_EVADE_JIT_COMPILE 4
#define JOCKY_EVADE_POLYMORPHIC 5

int jocky_detect_debugger(void);

int jocky_detect_ida(void);

int jocky_detect_valgrind(void);

int jocky_detect_strace(void);

int jocky_detect_gdbserver(void);

int jocky_detect_ptrace(void);

int jocky_check_environment_modified(void);

int jocky_check_memory_breakpoints(void);

int jocky_enable_anti_debugger_traps(void);

int jocky_enable_anti_trace_traps(void);

int jocky_hide_memory_region(void* addr, int size);

int jocky_unhide_memory_region(void* addr, int size);

int jocky_is_being_analyzed(void);

#endif
