/*
 * Unit Tests for Driver Intelligence Scoring Engine
 * Tests multi-tier scoring and fallback chain generation
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../../src/runtime/byovd/driver_scoring.h"

static void test_evasion_scoring()
{
    printf("Testing evasion scoring...\n");

    BYOVD_DRIVER_METADATA meta = {
        .name = "rtkiow10x64.sys",
        .is_signed = 1,
        .is_microsoft_blocked = 0,
        .is_on_edr_list = 0,
        .compilation_date = 2019
    };

    int score = byovd_score_driver_evasion("rtkiow10x64.sys", &meta);
    assert(score > 80 && score <= 100);
    printf("  rtkiow10x64 evasion score: %d (expected: 80-100)\n", score);

    meta.compilation_date = 2010;
    score = byovd_score_driver_evasion("rtkiow10x64.sys", &meta);
    assert(score > 80);
    printf("  Older driver score: %d (expected: > 80)\n", score);

    meta.is_microsoft_blocked = 1;
    score = byovd_score_driver_evasion("rtkiow10x64.sys", &meta);
    assert(score < 50);
    printf("  Blocked driver score: %d (expected: < 50)\n", score);

    printf("  PASS\n\n");
}

static void test_prevalence_scoring()
{
    printf("Testing prevalence scoring...\n");

    BYOVD_DRIVER_METADATA meta = {
        .name = "rtkiow10x64.sys",
        .is_signed = 1,
        .compilation_date = 2019
    };

    int score = byovd_score_driver_prevalence("rtkiow10x64.sys", &meta);
    assert(score > 75 && score <= 100);
    printf("  rtkiow10x64 prevalence score: %d (expected: 75-100)\n", score);

    score = byovd_score_driver_prevalence("speedfan.sys", &meta);
    assert(score < 80);
    printf("  speedfan prevalence score: %d (expected: < 80)\n", score);

    printf("  PASS\n\n");
}

static void test_capability_scoring()
{
    printf("Testing capability scoring...\n");

    uint32_t required_caps = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE | BYOVD_CAP_MSR_READ;

    int score = byovd_score_driver_capability("rtkiow10x64.sys", required_caps);
    assert(score > 85 && score <= 100);
    printf("  rtkiow10x64 capability score (phys+msr): %d (expected: 85-100)\n", score);

    score = byovd_score_driver_capability("nvflsh64.sys", required_caps);
    assert(score > 0 && score <= 100);
    printf("  nvflsh64 capability score (missing MSR): %d\n", score);

    required_caps = BYOVD_CAP_PHYS_READ;
    score = byovd_score_driver_capability("speedfan.sys", required_caps);
    assert(score > 50);
    printf("  speedfan capability score (phys only): %d\n", score);

    printf("  PASS\n\n");
}

static void test_blocklist_scoring()
{
    printf("Testing blocklist scoring...\n");

    BYOVD_DRIVER_METADATA meta = {
        .name = "rtkiow10x64.sys",
        .is_microsoft_blocked = 0,
        .is_on_edr_list = 0,
        .yara_matches = 0,
        .sig_revoked = 0
    };

    int score = byovd_score_driver_blocklist("rtkiow10x64.sys", &meta);
    assert(score > 80);
    printf("  Clean driver blocklist score: %d (expected: > 80)\n", score);

    meta.is_microsoft_blocked = 1;
    score = byovd_score_driver_blocklist("rtkiow10x64.sys", &meta);
    assert(score == 25);
    printf("  Microsoft-blocked driver score: %d (expected: 25)\n", score);

    meta.is_microsoft_blocked = 0;
    meta.is_on_edr_list = 1;
    score = byovd_score_driver_blocklist("rtkiow10x64.sys", &meta);
    assert(score == 30);
    printf("  EDR-listed driver score: %d (expected: 30)\n", score);

    meta.is_on_edr_list = 0;
    meta.yara_matches = 3;
    score = byovd_score_driver_blocklist("rtkiow10x64.sys", &meta);
    assert(score > 0 && score < 40);
    printf("  Driver with YARA matches: %d (expected: 0-40)\n", score);

    printf("  PASS\n\n");
}

static void test_composite_scoring()
{
    printf("Testing composite scoring...\n");

    BYOVD_DRIVER_METADATA meta = {
        .name = "rtkiow10x64.sys",
        .is_signed = 1,
        .is_microsoft_blocked = 0,
        .is_on_edr_list = 0,
        .compilation_date = 2019,
        .yara_matches = 0,
        .sig_revoked = 0
    };

    uint32_t required_caps = BYOVD_CAP_PHYS_READ | BYOVD_CAP_PHYS_WRITE;
    int score = byovd_score_driver_composite("rtkiow10x64.sys", &meta, required_caps);

    assert(score > 70);
    printf("  rtkiow10x64 composite score: %d (expected: > 70)\n", score);

    meta.is_microsoft_blocked = 1;
    score = byovd_score_driver_composite("rtkiow10x64.sys", &meta, required_caps);
    assert(score < 50);
    printf("  Blocked driver composite score: %d (expected: < 50)\n", score);

    printf("  PASS\n\n");
}

static void test_driver_selection()
{
    printf("Testing driver selection...\n");

    BYOVD_DRIVER_METADATA drivers[3] = {
        {
            .name = "rtkiow10x64.sys",
            .is_signed = 1,
            .is_microsoft_blocked = 0,
            .is_on_edr_list = 0,
            .compilation_date = 2019,
            .capability_count = 6
        },
        {
            .name = "speedfan.sys",
            .is_signed = 1,
            .is_microsoft_blocked = 0,
            .is_on_edr_list = 0,
            .compilation_date = 2008,
            .capability_count = 5
        },
        {
            .name = "blocked.sys",
            .is_signed = 1,
            .is_microsoft_blocked = 1,
            .is_on_edr_list = 0,
            .compilation_date = 2019,
            .capability_count = 4
        }
    };

    BYOVD_DRIVER_MANIFEST manifest = {
        .driver_count = 3,
        .drivers = drivers
    };

    uint32_t required_caps = BYOVD_CAP_PHYS_READ;
    int count = 0;

    BYOVD_DRIVER_CANDIDATE* candidates = byovd_select_driver(&manifest, required_caps, &count);
    assert(candidates != NULL);
    assert(count >= 2);
    printf("  Found %d valid candidates\n", count);

    assert(strcmp(candidates[0].metadata->name, "rtkiow10x64.sys") == 0 ||
           strcmp(candidates[0].metadata->name, "speedfan.sys") == 0);
    printf("  Top candidate: %s (score: %d)\n", candidates[0].metadata->name, candidates[0].score);

    for (int i = 1; i < count; i++) {
        assert(candidates[i].score <= candidates[i-1].score);
        printf("  Candidate %d: %s (score: %d)\n", i+1, candidates[i].metadata->name, candidates[i].score);
    }

    assert(strcmp(candidates[count-1].metadata->name, "blocked.sys") != 0);
    printf("  Blocked driver correctly filtered out\n");

    free(candidates);
    printf("  PASS\n\n");
}

static void test_fallback_chain()
{
    printf("Testing fallback chain generation...\n");

    BYOVD_DRIVER_METADATA drivers[4] = {
        {
            .name = "driver1.sys",
            .is_signed = 1,
            .is_microsoft_blocked = 0,
            .is_on_edr_list = 0,
            .compilation_date = 2020,
            .capability_count = 4
        },
        {
            .name = "driver2.sys",
            .is_signed = 1,
            .is_microsoft_blocked = 0,
            .is_on_edr_list = 0,
            .compilation_date = 2018,
            .capability_count = 4
        },
        {
            .name = "driver3.sys",
            .is_signed = 1,
            .is_microsoft_blocked = 0,
            .is_on_edr_list = 0,
            .compilation_date = 2016,
            .capability_count = 4
        },
        {
            .name = "driver4.sys",
            .is_signed = 1,
            .is_microsoft_blocked = 0,
            .is_on_edr_list = 0,
            .compilation_date = 2014,
            .capability_count = 4
        }
    };

    BYOVD_DRIVER_MANIFEST manifest = {
        .driver_count = 4,
        .drivers = drivers
    };

    uint32_t required_caps = BYOVD_CAP_PHYS_READ;
    int chain_count = 0;

    const char** chain = byovd_get_driver_fallback_chain(&manifest, required_caps, 3, &chain_count);
    assert(chain != NULL);
    assert(chain_count == 3);
    printf("  Generated fallback chain with %d drivers\n", chain_count);

    for (int i = 0; i < chain_count; i++) {
        printf("  Fallback[%d]: %s\n", i, chain[i]);
    }

    free(chain);
    printf("  PASS\n\n");
}

static void test_edge_cases()
{
    printf("Testing edge cases...\n");

    BYOVD_DRIVER_METADATA empty_drivers[] = {};
    BYOVD_DRIVER_MANIFEST empty_manifest = {
        .driver_count = 0,
        .drivers = empty_drivers
    };

    int count = 0;
    BYOVD_DRIVER_CANDIDATE* candidates = byovd_select_driver(&empty_manifest, BYOVD_CAP_PHYS_READ, &count);
    assert(count == 0 || candidates == NULL);
    printf("  Empty manifest: OK\n");

    BYOVD_DRIVER_METADATA meta = {
        .name = NULL,
        .compilation_date = 2019
    };

    int score = byovd_score_driver_evasion("unknown_driver.sys", &meta);
    assert(score == 50);
    printf("  Unknown driver default score: %d (expected: 50)\n", score);

    const char* best = byovd_select_best_driver(NULL, BYOVD_CAP_PHYS_READ);
    assert(best == NULL);
    printf("  NULL manifest returns NULL: OK\n");

    printf("  PASS\n\n");
}

int main()
{
    printf("=== Driver Scoring Engine Tests ===\n\n");

    test_evasion_scoring();
    test_prevalence_scoring();
    test_capability_scoring();
    test_blocklist_scoring();
    test_composite_scoring();
    test_driver_selection();
    test_fallback_chain();
    test_edge_cases();

    printf("=== All Tests Passed ===\n");
    return 0;
}
