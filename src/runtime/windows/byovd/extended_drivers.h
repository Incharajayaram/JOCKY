/*
 * JOCKY Extended BYOVD Driver Support
 * Header for 346+ unblocked driver manifest
 * Authorized: Red Hat + IIT Bombay Cyber Security Team
 */

#ifndef JOCKY_EXTENDED_DRIVERS_H
#define JOCKY_EXTENDED_DRIVERS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Driver entry in manifest */
typedef struct {
    const char* name;
    const char* device_path;
    uint32_t ioctl_base;
    float reliability_score;
} JOCKY_DRIVER_ENTRY;

/* Load extended driver manifest from JSON file */
int jocky_load_extended_manifest(const char* json_path);

/* Select best driver from manifest based on reliability */
int jocky_select_best_extended_driver(
    JOCKY_DRIVER_ENTRY* out_driver);

/* Try extended driver chain with fallback */
int jocky_try_extended_driver_chain(
    JOCKY_DRIVER_ENTRY* driver_chain,
    uint32_t chain_length,
    uint32_t* out_working_index);

/* Find driver by name */
int jocky_get_driver_by_name(
    const char* driver_name,
    JOCKY_DRIVER_ENTRY* out_driver);

/* List all available drivers */
int jocky_list_available_drivers(
    JOCKY_DRIVER_ENTRY* out_drivers,
    uint32_t max_count,
    uint32_t* out_count);

/* Get manifest statistics */
int jocky_get_manifest_stats(
    uint32_t* out_total_count,
    float* out_avg_reliability,
    uint32_t* out_kernel_access_count);

/* Validate IOCTL code for driver */
int jocky_validate_driver_ioctl(
    const JOCKY_DRIVER_ENTRY* driver,
    uint32_t test_ioctl);

/* Export manifest as C array */
int jocky_export_manifest_as_c_array(
    const char* output_path);

#ifdef __cplusplus
}
#endif

#endif
