#include <windows.h>
#include <stdint.h>
#include <stdio.h>

#define STUB_FUNC(ret, name, ...) ret name(__VA_ARGS__) { return 0; }
#define STUB_VOID(name, ...) void name(__VA_ARGS__) { }

STUB_FUNC(int32_t, sandbox_set_limits, int32_t pid, int64_t mem_limit, int32_t time_limit, int64_t cpu_limit)
STUB_FUNC(int32_t, sandbox_spawn, const char* exe, const char* args)
STUB_FUNC(int32_t, sandbox_monitor, int32_t pid)
STUB_FUNC(int32_t, sandbox_wait, int32_t pid)
STUB_FUNC(int32_t, sandbox_export_trace, int32_t pid, const char* path)

STUB_FUNC(int32_t, crypto_generate_key, int32_t size)
STUB_FUNC(int32_t, crypto_aes256_encrypt, int8_t* data, int32_t size, int8_t* key)
STUB_FUNC(int32_t, crypto_aes256_decrypt, int8_t* data, int32_t size, int8_t* key)

STUB_FUNC(int32_t, ai_init)
STUB_FUNC(int32_t, ai_collect_telemetry)
STUB_FUNC(double, ai_score_threat)

STUB_FUNC(int32_t, fs_exists, const char* path)
STUB_FUNC(int32_t, fs_list_files, const char* path, int32_t recursive)
STUB_FUNC(int64_t, fs_file_size, const char* path)
STUB_FUNC(int8_t*, fs_read_file, const char* path)
STUB_FUNC(int32_t, fs_write_file, const char* path, int8_t* data, int64_t size)

STUB_FUNC(int32_t, audit_init, int32_t buffer_size)
STUB_FUNC(int32_t, audit_log, const char* category, const char* action, const char* detail, const char* result)
STUB_FUNC(int32_t, audit_export, const char* path)
STUB_FUNC(int32_t, audit_verify)

STUB_VOID(provenance_record, const char* path, const char* operation, const char* detail)

STUB_FUNC(int32_t, forensics_wipe_cmd_history)
STUB_FUNC(int32_t, forensics_wipe_powershell_history)
STUB_FUNC(int32_t, forensics_flush_arp_cache)
STUB_FUNC(int32_t, forensics_clear_dns_cache)

STUB_FUNC(int32_t, jocky_cleanup_event_logs, const char* channels)
STUB_FUNC(int32_t, jocky_cleanup_usn_journal)

STUB_FUNC(int32_t, byovd_load_driver, const char* name)
STUB_FUNC(int32_t, byovd_test_exploit, int32_t handle)
STUB_FUNC(int32_t, btr_disable_notifications)
STUB_FUNC(int32_t, btr_mask_module, const char* dll_name)

STUB_FUNC(int32_t, edrhoker_detect)
STUB_FUNC(int32_t, blindside_unhook_ntdll)

STUB_FUNC(int32_t, exfil_dns_tunnel, const char* domain, const char* metadata)
STUB_FUNC(int32_t, exfil_discord_webhook, const char* webhook, const char* message)
STUB_FUNC(int32_t, exfil_local_cdn, const char* endpoint, const char* name, const char* token, const char* metadata)

STUB_FUNC(int32_t, jocky_registry_create_key, int32_t hive, const char* path, int8_t* out_handle)
STUB_FUNC(int32_t, jocky_registry_set_value, int8_t* handle, const char* name, const char* value, int32_t type_, int32_t flags)
STUB_FUNC(int32_t, jocky_registry_close_key, int8_t* handle)

STUB_FUNC(int32_t, jocky_exploit_disable_callbacks)
STUB_FUNC(int32_t, jocky_exploit_token_replacement, int32_t arg1, int32_t arg2)
STUB_FUNC(int32_t, jocky_sleep_and_recheck)

STUB_FUNC(int32_t, jocky_module_load, const char* path)
STUB_FUNC(int32_t, jocky_module_unload, int8_t* handle)
STUB_FUNC(int32_t, jocky_module_has_symbol, int8_t* handle, const char* symbol)
STUB_FUNC(int64_t, jocky_module_base, const char* path)
STUB_FUNC(int8_t*, jocky_module_resolve_symbol, int8_t* handle, const char* symbol)

STUB_FUNC(int32_t, jocky_process_hollow_linux, int32_t pid, int8_t* payload, int64_t size)
STUB_FUNC(int32_t, jocky_process_ptrace_attach, int32_t pid)
STUB_FUNC(int32_t, jocky_process_ptrace_detach, int32_t pid)
STUB_FUNC(int32_t, jocky_process_get_maps, int32_t pid, int8_t* buf, int64_t bufsize)

STUB_FUNC(int32_t, jocky_fence2pwn_detect_kfence)
STUB_FUNC(int32_t, jocky_fence2pwn_get_pool_info, int8_t* info)
STUB_FUNC(int32_t, jocky_fence2pwn_trigger_allocations, int64_t size, int32_t count)
STUB_FUNC(int32_t, jocky_fence2pwn_exploit_uaf, int32_t alloc_idx, int32_t free_idx, int8_t* data)
STUB_FUNC(int32_t, jocky_fence2pwn_manipulate_creds, int32_t uid, int32_t gid)
STUB_FUNC(int32_t, jocky_fence2pwn_allocate_cred_objects, int32_t count)
STUB_FUNC(int32_t, jocky_fence2pwn_write_cred, int8_t* addr, int32_t uid, int32_t gid)
STUB_FUNC(int32_t, jocky_fence2pwn_trigger_reclamation)
STUB_FUNC(int32_t, jocky_fence2pwn_elevate_to_root)
STUB_FUNC(int32_t, jocky_fence2pwn_find_uaf_primitive)

STUB_FUNC(int32_t, jocky_ebpf_load, int8_t* prog, int32_t prog_size, int32_t prog_type)
STUB_FUNC(int32_t, jocky_ebpf_attach, int32_t prog_fd, int32_t attach_type, int32_t target_fd)
STUB_FUNC(int64_t, jocky_ebpf_run, int32_t prog_fd, int8_t* ctx, int32_t ctx_size)

STUB_FUNC(int32_t, jocky_lkm_load, const char* path, const char* module_name)
STUB_FUNC(int32_t, jocky_lkm_unload, const char* module_name)
STUB_FUNC(int8_t*, jocky_lkm_get_symbol, const char* module_name, const char* symbol)

STUB_FUNC(int32_t, jocky_syscall_hook, int32_t syscall_num, int8_t* handler)
STUB_FUNC(int32_t, jocky_syscall_unhook, int32_t syscall_num)
STUB_FUNC(int32_t, jocky_syscall_trace, int32_t pid)

STUB_FUNC(int32_t, jocky_ftrace_attach, const char* func_name, int8_t* callback)
STUB_FUNC(int32_t, jocky_ftrace_detach, const char* func_name)

STUB_FUNC(int32_t, linux_forensics_wipe_bash_history)
STUB_FUNC(int32_t, jocky_linux_cleanup_syslog)
STUB_FUNC(int32_t, jocky_linux_cleanup_journal)

STUB_FUNC(int32_t, ptrace, int32_t request, int32_t pid, void* addr, void* data)

STUB_FUNC(int32_t, jocky_driver_map_kernel, int8_t* ctx, int64_t phys_addr, int32_t size)
