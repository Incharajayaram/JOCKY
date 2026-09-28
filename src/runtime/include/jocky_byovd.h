#ifndef JOCKY_BYOVD_H
#define JOCKY_BYOVD_H

#include <stdint.h>

/* Bring Your Own Vulnerable Driver (BYOVD) driver loading and selection
 *
 * Automatically loads and selects the best vulnerable driver for exploit chain
 * based on Windows version, installed drivers, and vulnerability signatures.
 */

typedef struct {
    char name[256];          /* Driver name */
    char filename[512];      /* Driver file path */
    char version[32];        /* Driver version */
    uint32_t signature;      /* Vulnerability signature */
    float exploit_reliability; /* Estimated success rate */
    int target_os_min;       /* Minimum OS version (10-20) */
    int target_os_max;       /* Maximum OS version */
    char cve[32];            /* Associated CVE */
} JOCKY_DRIVER_MANIFEST;

/* Load driver from manifest */
int jocky_byovd_load_from_manifest(const JOCKY_DRIVER_MANIFEST* manifest, int* out_handle);

/* Select best driver for current system */
int jocky_byovd_select_best_driver(
    const JOCKY_DRIVER_MANIFEST* manifests,
    uint32_t manifest_count,
    JOCKY_DRIVER_MANIFEST* out_selected);

/* Load next fallback driver if current fails */
int jocky_byovd_fallback_next(int current_handle, int* out_next_handle);

/* Query driver status */
int jocky_byovd_query_status(int handle, int* out_loaded, char* out_version);

/* Unload driver safely */
int jocky_byovd_unload(int handle);

/* Get system Windows version for driver selection */
int jocky_byovd_get_os_version(void);

/* Test driver exploit capability */
int jocky_byovd_test_exploit(int handle, int* out_success);

#endif
