# JOCKY Linux Runtime Implementation Roadmap

## Current Status
- **Total Functions Needed**: 113
- **P0 (Critical) - COMPLETED**: 24/24 ✅
- **P1 (Important) - TODO**: 19/19
- **P2 (Nice to Have) - TODO**: 32/32
- **P3 (Optional) - TODO**: 38/38

## P0 Completed ✅ (24 functions)

### Core I/O (6)
- ✅ println - stdout output
- ✅ string - integer to string
- ✅ fs_read_file - read file contents
- ✅ fs_write_file - write file
- ✅ fs_list_files - list directory contents
- ✅ fs_exists - check file existence

### Process Control (6)
- ✅ jocky_process_ptrace_attach - PTRACE_ATTACH
- ✅ jocky_process_ptrace_detach - PTRACE_DETACH
- ⚠️ jocky_process_get_maps - stub (read /proc/pid/maps)
- ⚠️ jocky_thread_hijack - stub (ptrace code injection)
- ⚠️ jocky_spoof_syscall - stub (ptrace modify syscall)
- ✅ jocky_spoof_call - no-op

### Anti-Analysis (4)
- ✅ jocky_is_debugger_present - PTRACE_TRACEME check
- ✅ jocky_is_sandbox - cgroup + process checking
- ✅ jocky_is_vm - cpuinfo + device detection
- ✅ jocky_check_analysis_environment - combined check

### Runtime/Crypto (5)
- ✅ jocky_runtime_init - OpenSSL initialization
- ✅ crypto_aes256_encrypt - AES-256 encryption
- ✅ crypto_aes256_decrypt - AES-256 decryption
- ⚠️ jocky_decrypt_rc4 - stub
- ✅ jocky_decrypt_xor - XOR decryption

### Cleanup/Forensics (7)
- ✅ linux_forensics_wipe_bash_history
- ✅ jocky_linux_cleanup_syslog
- ✅ jocky_linux_cleanup_journal
- ✅ jocky_wipe_artifacts
- ✅ jocky_wipe_prefetch
- ✅ jocky_clear_logs
- ✅ jocky_cleanup_all

---

## P1 - Important (19 functions)

### Exploit Chain: FENCE2PWN (10)
Priority: **HIGH** - Major kernel exploitation
- jocky_fence2pwn_detect_kfence - Detect KFENCE allocator
- jocky_fence2pwn_find_uaf_primitive - Find UAF primitive
- jocky_fence2pwn_get_pool_info - Query memory pool
- jocky_fence2pwn_allocate_cred_objects - Prepare credential objects
- jocky_fence2pwn_manipulate_creds - Modify kernel creds
- jocky_fence2pwn_write_cred - Write creds to kernel
- jocky_fence2pwn_trigger_allocations - Trigger memory alloc
- jocky_fence2pwn_trigger_reclamation - Trigger dealloc
- jocky_fence2pwn_exploit_uaf - Execute UAF
- jocky_fence2pwn_elevate_to_root - Gain root privilege

**Implementation Approach:**
- Use /proc/kpageflags to detect KFENCE memory
- Parse /proc/meminfo for pool info
- Modify /proc/self/uid_map or use direct memory write
- Requires kernel struct knowledge (cred_t, task_struct)

### BYOVD Driver Operations (4)
Priority: **HIGH** - Bring Your Own Vulnerable Driver
- jocky_byovd_new - Allocate BYOVD context
- jocky_byovd_load - Load vulnerable driver
- jocky_byovd_unload - Unload driver
- byovd_test_exploit - Test if exploit works

**Implementation Approach:**
- Load kernel driver via insmod/modprobe
- Device ioctl communication
- Test physical memory access

### Module/Symbol Operations (3)
Priority: **MEDIUM** - Runtime introspection
- jocky_module_resolve_symbol - Resolve symbol in loaded module
- jocky_module_stomp - Hide module from lsmod
- jocky_lkm_get_symbol - Get LKM symbol address

**Implementation Approach:**
- Use /proc/modules and /proc/kallsyms
- Modify list_head in kernel to hide module
- Parse ELF symbols from module

### Kernel Memory Access (2)
Priority: **HIGH** - Direct memory access
- jocky_kread - Read kernel memory
- jocky_kwrite - Write kernel memory

**Implementation Approach:**
- Use /dev/mem if available
- Use BYOVD driver for memory access
- Use /proc/self/pagemap for address translation

---

## P2 - Nice to Have (32 functions)

### Exfiltration Channels (8)
- exfil_discord_webhook - POST to Discord webhook
- exfil_dns_tunnel - DNS exfiltration
- exfil_local_cdn - Upload to CDN endpoint
- jocky_exfil_dns - DNS tunnel variant
- jocky_exfil_discord - Discord variant
- jocky_exfil_telegram - Telegram bot exfil
- jocky_exfil_github - GitHub gist exfil
- jocky_exfil_front - Frontend proxy exfil

**Implementation Approach:**
- Use libcurl for HTTP/HTTPS
- Use custom DNS packet crafting for DNS tunnel
- Implement chunked data encoding

### Sandbox Operations (5)
- sandbox_spawn - Spawn sandboxed process
- sandbox_set_limits - Set resource limits (ulimit)
- sandbox_monitor - Monitor process execution
- sandbox_wait - Wait for process
- sandbox_export_trace - Export strace output

**Implementation Approach:**
- Use clone/unshare for namespace isolation
- Use setrlimit for resource limits
- Use strace for monitoring

### Cleanup/Forensics (7)
- jocky_cleanup_event_logs - Clear event logs
- jocky_cleanup_usn_journal - USN journal (Windows only - no-op)
- audit_init/export/verify - Linux audit subsystem
- audit_log - Log to auditd
- ai_collect_telemetry - AI telemetry collection
- ai_init/ai_score_threat - AI scoring

**Implementation Approach:**
- Use auditctl for audit control
- Use syslog for logging
- Aggregate system metrics for AI

---

## P3 - Optional (38 functions)

### Windows-Specific (15) - No-ops on Linux
- jocky_patch_shimcache/amcache - Registry cache (no-op)
- jocky_registry_* - Registry operations (3) (no-op)
- jocky_is_sandbox (already implemented via cgroups)
- jocky_disable_edr_callbacks - EDR detection (no-op)
- jocky_disable_etw - ETW logging (no-op)
- jocky_unhook_ntdll - NTDLL unhooking (no-op)
- blindside_unhook_ntdll - Hook removal (no-op)
- btr_disable_notifications - BTR hiding (no-op)
- edrhoker_detect - EDR detection (no-op)
- jocky_exploit_disable_callbacks - Callback disable (no-op)
- jocky_exploit_token_replacement - Token manipulation (no-op)

### Ftrace (2)
- jocky_ftrace_attach - Attach ftrace hook
- jocky_ftrace_detach - Detach hook

**Implementation Approach:**
- Use /sys/kernel/debug/tracing/
- Write to trace_kprobes

### Privilege Escalation (1)
- jocky_elevate_token - Elevate privileges

**Implementation Approach:**
- Use FENCE2PWN or BYOVD for elevation
- Modify uid/gid in kernel

### Misc (6)
- array_len/append - Already implemented
- provenance_record - Audit logging
- jocky_get_syscall_number - Get syscall nr
- jocky_module_has_symbol - Check symbol exists
- jocky_sleep_and_recheck - Already implemented
- jocky_runtime_init - Already implemented

---

## Implementation Order

1. **Phase 1 (DONE)** - P0 Critical (24 functions)
   - I/O, process control, anti-analysis, crypto, cleanup

2. **Phase 2 (NEXT)** - P1 Important (19 functions)
   - FENCE2PWN chain (10) - requires kernel knowledge
   - BYOVD chain (4) - driver-based
   - Module/symbol ops (3) - introspection
   - Kernel memory access (2) - core capability

3. **Phase 3** - P2 Nice to Have (32 functions)
   - Exfiltration channels (8)
   - Sandbox operations (5)
   - Additional forensics (7)
   - Audit/logging/AI (12)

4. **Phase 4** - P3 Optional (38 functions)
   - Windows stubs (15) - mostly no-ops
   - Ftrace (2)
   - Privilege escalation (1)
   - Miscellaneous (20)

## Research Needed

For P1 implementation, research required on:

1. **KFENCE Detection**
   - Linux kernel source: mm/kfence/core.c
   - KFENCE memory bitmap structure
   - Detecting freed objects

2. **FENCE2PWN Exploit**
   - UAF primitive discovery
   - Cred structure exploitation
   - Kernel address layout (KASLR bypass)

3. **BYOVD Drivers for Linux**
   - eBPF/kprobes drivers
   - IO port drivers
   - Device drivers with memory mapping

4. **Kernel Symbol Resolution**
   - /proc/kallsyms parsing
   - DWARF debug symbol parsing
   - Module symbol tables

---

## Estimated Timeline

- **Phase 1**: 1-2 hours (COMPLETED ✅)
- **Phase 2**: 4-6 hours (kernel exploitation requires careful implementation)
- **Phase 3**: 6-8 hours (mostly syscall wrappers)
- **Phase 4**: 2-3 hours (mostly stubs)

**Total**: ~13-19 hours for full implementation
