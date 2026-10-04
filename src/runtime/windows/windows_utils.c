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

/* Sleep/Recheck */
void jocky_sleep_and_recheck(void) {
#ifdef _WIN32
    Sleep(100);
#else
    usleep(100000);
#endif
}

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
int32_t blindside_unhook_ntdll(void) { return -1; }

/* Exfiltration */
int32_t exfil_dns_tunnel(void) { return -1; }
int32_t exfil_discord_webhook(void) { return -1; }
int32_t exfil_local_cdn(void) { return -1; }

/* BTR/BYOVD */
int32_t btr_disable_notifications(void) { return -1; }
int32_t btr_mask_module(const char* dll_name) { return -1; }

/* Crypto */
int32_t crypto_generate_key(void) { return -1; }
int32_t crypto_aes256_encrypt(void* key, void* plaintext, void* ciphertext) { return -1; }
int32_t crypto_aes256_decrypt(void* key, void* ciphertext, void* plaintext) { return -1; }

/* AI/Telemetry */
int32_t ai_init(void) { return -1; }
int32_t ai_score_threat(void) { return -1; }
int32_t ai_collect_telemetry(void) { return -1; }

/* Provenance/Telemetry */
int32_t provenance_record(void) { return -1; }

/* Common string operation used by compiler */
char* jocky_str_concat(void* a, void* b) {
    return string_concat((const char*)a, (const char*)b);
}

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
