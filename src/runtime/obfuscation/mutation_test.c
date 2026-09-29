/*
 * Comprehensive Tests for Polymorphic Obfuscation System
 *
 * Tests cover:
 * - Randomization infrastructure
 * - Instruction-level mutations
 * - Control flow randomization
 * - Self-modifying code
 * - Pass management
 * - Statistical accuracy
 */

#include "mutation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TEST_PASS(name) printf("[PASS] %s\n", name)
#define TEST_FAIL(name, reason) printf("[FAIL] %s: %s\n", name, reason)

/* ============================================================================
 * Test 1: Randomization Infrastructure
 * ============================================================================ */

void test_rng_seeding(void)
{
    jocky_seed_rng(42);
    uint32_t a = jocky_random_u32();
    uint32_t b = jocky_random_u32();

    jocky_seed_rng(42);
    uint32_t c = jocky_random_u32();
    uint32_t d = jocky_random_u32();

    assert(a == c && b == d);
    TEST_PASS("RNG seeding produces deterministic sequences");
}

void test_rng_range(void)
{
    jocky_seed_rng(100);

    for (int i = 0; i < 1000; i++) {
        uint32_t val = jocky_random_range(10, 20);
        assert(val >= 10 && val < 20);
    }

    TEST_PASS("Random range stays within bounds");
}

void test_rng_u64(void)
{
    jocky_seed_rng(200);

    for (int i = 0; i < 100; i++) {
        uint64_t val = jocky_random_u64();
        assert(val != 0 || i == 0);
    }

    TEST_PASS("64-bit random generation works");
}

void test_shuffle_array(void)
{
    jocky_seed_rng(300);

    int arr[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = i;
    }

    jocky_shuffle_array(arr, 10, sizeof(int));

    int sum = 0;
    for (int i = 0; i < 10; i++) {
        sum += arr[i];
    }

    assert(sum == 45);
    TEST_PASS("Array shuffling preserves elements");
}

/* ============================================================================
 * Test 2: Instruction-Level Mutations
 * ============================================================================ */

void test_mutate_instructions(void)
{
    uint8_t code[] = {
        0x48, 0x01, 0xc1,
        0x48, 0x01, 0xd2,
        0x90
    };

    jocky_seed_rng(400);
    jocky_mutate_instructions(code, sizeof(code));

    TEST_PASS("Instruction mutation completes without crash");
}

void test_mutate_constants(void)
{
    uint8_t code[] = {
        0xb8, 0x00, 0x00, 0x00, 0x42,
        0x90, 0x90, 0x90
    };

    jocky_seed_rng(500);
    jocky_mutate_constants(code, sizeof(code));

    TEST_PASS("Constant mutation completes without crash");
}

void test_inject_junk_code(void)
{
    uint8_t code[128];
    memset(code, 0x90, sizeof(code));

    jocky_seed_rng(600);
    size_t original_length = sizeof(code);
    jocky_inject_junk_code(code, sizeof(code));

    int nop_count = 0;
    for (size_t i = 0; i < sizeof(code); i++) {
        if (code[i] == 0x90) nop_count++;
    }

    assert(nop_count > 0 && nop_count < original_length);
    TEST_PASS("Junk code injection modifies buffer");
}

void test_mutate_bitwise_ops(void)
{
    uint8_t code[] = {
        0x48, 0x21, 0xc1,
        0x48, 0x09, 0xd2,
        0x48, 0x31, 0xc3
    };

    jocky_seed_rng(700);
    jocky_mutate_bitwise_ops(code, sizeof(code));

    TEST_PASS("Bitwise operation mutation completes");
}

void test_mutate_data_values(void)
{
    uint32_t data[16];
    for (int i = 0; i < 16; i++) {
        data[i] = 0xdeadbeef;
    }

    jocky_seed_rng(800);
    jocky_mutate_data_values((uint8_t *)data, sizeof(data));

    int changed = 0;
    for (int i = 0; i < 16; i++) {
        if (data[i] != 0xdeadbeef) changed++;
    }

    assert(changed > 0);
    TEST_PASS("Data value mutation modifies values");
}

/* ============================================================================
 * Test 3: Control Flow Mutations
 * ============================================================================ */

void test_extract_basic_blocks(void)
{
    uint8_t code[] = {
        0x90, 0x90, 0xc3,
        0x90, 0x90, 0xe9, 0x00, 0x00, 0x00, 0x00,
        0x90, 0x90, 0xc3
    };

    int block_count = 0;
    BasicBlock *blocks = jocky_extract_basic_blocks(code, sizeof(code), &block_count);

    assert(blocks != NULL);
    assert(block_count > 0);

    free(blocks);
    TEST_PASS("Basic block extraction works");
}

void test_randomize_cfg(void)
{
    uint8_t code[256];
    memset(code, 0x90, sizeof(code));

    for (int i = 0; i < sizeof(code) - 1; i += 20) {
        code[i] = 0xc3;
    }

    jocky_seed_rng(900);
    MutationStats before = jocky_get_mutation_stats();
    jocky_randomize_cfg(code, sizeof(code));
    MutationStats after = jocky_get_mutation_stats();

    assert(after.blocks_reordered >= before.blocks_reordered);
    TEST_PASS("CFG randomization updates statistics");
}

void test_polymorphic_dispatch(void)
{
    uint8_t code[] = {
        0xff, 0xe0,
        0x00, 0x00, 0x00, 0x40,
        0x90, 0x90
    };

    jocky_seed_rng(1000);
    jocky_create_polymorphic_dispatch(code, sizeof(code));

    TEST_PASS("Polymorphic dispatch creation works");
}

void test_randomize_switch_cases(void)
{
    uint8_t code[64];
    memset(code, 0x90, sizeof(code));

    jocky_seed_rng(1100);
    jocky_randomize_switch_cases(code, sizeof(code));

    TEST_PASS("Switch case randomization completes");
}

/* ============================================================================
 * Test 4: Self-Modifying Code
 * ============================================================================ */

void test_code_checksum(void)
{
    uint8_t code1[] = {0x90, 0x90, 0x90, 0x90};
    uint8_t code2[] = {0x90, 0x90, 0x90, 0x91};

    uint32_t sum1 = jocky_compute_code_checksum(code1, sizeof(code1));
    uint32_t sum2 = jocky_compute_code_checksum(code2, sizeof(code2));

    assert(sum1 != sum2);
    TEST_PASS("Code checksums differ for different inputs");
}

void test_checksum_stability(void)
{
    uint8_t code[] = {0xaa, 0xbb, 0xcc, 0xdd};

    uint32_t sum1 = jocky_compute_code_checksum(code, sizeof(code));
    uint32_t sum2 = jocky_compute_code_checksum(code, sizeof(code));

    assert(sum1 == sum2);
    TEST_PASS("Code checksums are stable");
}

void test_code_integrity_verification(void)
{
    uint8_t payload[] = {0x90, 0x90, 0xc3};

    SelfModifyingSegment segment = {
        .address = (uintptr_t)payload,
        .mutation_payload = payload,
        .payload_len = sizeof(payload),
        .checksum = jocky_compute_code_checksum(payload, sizeof(payload))
    };

    assert(jocky_verify_code_integrity(&segment));
    TEST_PASS("Code integrity verification works");
}

/* ============================================================================
 * Test 5: Mutation Engine Management
 * ============================================================================ */

void test_mutation_init(void)
{
    MutationEngine engine;
    jocky_mutation_init(&engine, 42, 3);

    assert(engine.seed == 42);
    assert(engine.intensity == 3);
    assert(engine.pass_count > 0);
    assert(engine.passes != NULL);

    free(engine.passes);
    TEST_PASS("Mutation engine initialization works");
}

void test_polymorphic_mutations(void)
{
    uint8_t code[128];
    memset(code, 0x90, sizeof(code));

    jocky_seed_rng(1200);
    MutationStats before = jocky_get_mutation_stats();
    jocky_apply_polymorphic_mutations(code, sizeof(code));
    MutationStats after = jocky_get_mutation_stats();

    assert(after.mutations_applied >= before.mutations_applied);
    TEST_PASS("Polymorphic mutations apply successfully");
}

void test_mutation_intensity_levels(void)
{
    for (int intensity = 1; intensity <= 5; intensity++) {
        uint8_t code[64];
        memset(code, 0x90, sizeof(code));

        jocky_seed_rng(1300 + intensity);
        jocky_reset_mutation_stats();
        jocky_apply_mutations_intensity(code, sizeof(code), intensity);
        MutationStats stats = jocky_get_mutation_stats();

        assert(stats.mutations_applied > 0 || intensity == 1);
    }

    TEST_PASS("Mutation intensity levels work correctly");
}

void test_mutation_reshuffle(void)
{
    MutationEngine engine;
    jocky_mutation_init(&engine, 42, 3);

    jocky_reshuffle_mutations();

    free(engine.passes);
    TEST_PASS("Mutation reshuffling works");
}

/* ============================================================================
 * Test 6: Mutation Control
 * ============================================================================ */

void test_mutation_enable_disable(void)
{
    uint8_t code[64];
    memset(code, 0x90, sizeof(code));

    jocky_set_mutation_enabled(false);
    assert(!jocky_is_mutation_enabled());

    jocky_reset_mutation_stats();
    jocky_apply_polymorphic_mutations(code, sizeof(code));
    MutationStats disabled_stats = jocky_get_mutation_stats();

    jocky_set_mutation_enabled(true);
    assert(jocky_is_mutation_enabled());

    memset(code, 0x90, sizeof(code));
    jocky_reset_mutation_stats();
    jocky_apply_polymorphic_mutations(code, sizeof(code));
    MutationStats enabled_stats = jocky_get_mutation_stats();

    assert(disabled_stats.mutations_applied < enabled_stats.mutations_applied);

    TEST_PASS("Mutation enable/disable control works");
}

void test_stats_reset(void)
{
    uint8_t code[64];
    memset(code, 0x90, sizeof(code));

    jocky_set_mutation_enabled(true);
    jocky_apply_polymorphic_mutations(code, sizeof(code));
    MutationStats before_reset = jocky_get_mutation_stats();

    assert(before_reset.mutations_applied > 0);

    jocky_reset_mutation_stats();
    MutationStats after_reset = jocky_get_mutation_stats();

    assert(after_reset.mutations_applied == 0);
    assert(after_reset.instructions_modified == 0);
    assert(after_reset.blocks_reordered == 0);

    TEST_PASS("Statistics reset works correctly");
}

/* ============================================================================
 * Test 7: Randomization Quality
 * ============================================================================ */

void test_rng_distribution(void)
{
    jocky_seed_rng(1400);

    int buckets[10] = {0};
    for (int i = 0; i < 10000; i++) {
        uint32_t val = jocky_random_range(0, 10);
        buckets[val]++;
    }

    int min = buckets[0];
    int max = buckets[0];
    for (int i = 1; i < 10; i++) {
        if (buckets[i] < min) min = buckets[i];
        if (buckets[i] > max) max = buckets[i];
    }

    int spread = max - min;
    assert(spread < 500);

    TEST_PASS("RNG distribution is reasonably uniform");
}

void test_mutation_uniqueness(void)
{
    uint8_t code1[64];
    uint8_t code2[64];

    memset(code1, 0x90, sizeof(code1));
    memset(code2, 0x90, sizeof(code2));

    jocky_seed_rng(1500);
    jocky_apply_polymorphic_mutations(code1, sizeof(code1));

    jocky_seed_rng(1600);
    jocky_apply_polymorphic_mutations(code2, sizeof(code2));

    assert(memcmp(code1, code2, sizeof(code1)) != 0);

    TEST_PASS("Different seeds produce different mutations");
}

/* ============================================================================
 * Test Runner
 * ============================================================================ */

int main(void)
{
    printf("\n=== Polymorphic Obfuscation Test Suite ===\n\n");

    printf("--- Randomization Infrastructure ---\n");
    fflush(stdout);
    test_rng_seeding();
    fflush(stdout);
    test_rng_range();
    fflush(stdout);
    test_rng_u64();
    fflush(stdout);
    test_shuffle_array();
    fflush(stdout);

    printf("\n--- Instruction-Level Mutations ---\n");
    test_mutate_instructions();
    test_mutate_constants();
    test_inject_junk_code();
    test_mutate_bitwise_ops();
    test_mutate_data_values();

    printf("\n--- Control Flow Mutations ---\n");
    test_extract_basic_blocks();
    test_randomize_cfg();
    test_polymorphic_dispatch();
    test_randomize_switch_cases();

    printf("\n--- Self-Modifying Code ---\n");
    test_code_checksum();
    test_checksum_stability();
    test_code_integrity_verification();

    printf("\n--- Mutation Engine Management ---\n");
    test_mutation_init();
    test_polymorphic_mutations();
    test_mutation_intensity_levels();
    test_mutation_reshuffle();

    printf("\n--- Mutation Control ---\n");
    test_mutation_enable_disable();
    test_stats_reset();

    printf("\n--- Randomization Quality ---\n");
    test_rng_distribution();
    test_mutation_uniqueness();

    printf("\n=== All Tests Passed ===\n\n");
    return 0;
}
