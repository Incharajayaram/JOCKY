/*
 * Debug test - run the actual unit tests one by one
 */

#include "mutation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TEST_PASS(name) printf("[PASS] %s\n", name)

void test_rng_seeding(void)
{
    printf("test_rng_seeding...\n"); fflush(stdout);
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
    printf("test_rng_range...\n"); fflush(stdout);
    jocky_seed_rng(100);

    for (int i = 0; i < 1000; i++) {
        uint32_t val = jocky_random_range(10, 20);
        assert(val >= 10 && val < 20);
    }

    TEST_PASS("Random range stays within bounds");
}

void test_rng_u64(void)
{
    printf("test_rng_u64...\n"); fflush(stdout);
    jocky_seed_rng(200);

    for (int i = 0; i < 100; i++) {
        uint64_t val = jocky_random_u64();
        assert(val != 0 || i == 0);
    }

    TEST_PASS("64-bit random generation works");
}

void test_shuffle_array(void)
{
    printf("test_shuffle_array...\n"); fflush(stdout);
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

void test_mutate_instructions(void)
{
    printf("test_mutate_instructions...\n"); fflush(stdout);
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
    printf("test_mutate_constants...\n"); fflush(stdout);
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
    printf("test_inject_junk_code...\n"); fflush(stdout);
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

int main(void)
{
    printf("\n=== Debug Test Suite ===\n\n");

    printf("--- Randomization Infrastructure ---\n");
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
    fflush(stdout);
    test_mutate_constants();
    fflush(stdout);
    test_inject_junk_code();
    fflush(stdout);

    printf("\n=== All Debug Tests Passed ===\n\n");
    return 0;
}
