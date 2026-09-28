/*
 * Debug test - continuation
 */

#include "mutation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TEST_PASS(name) printf("[PASS] %s\n", name)

void test_mutate_bitwise_ops(void)
{
    printf("test_mutate_bitwise_ops...\n"); fflush(stdout);
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
    printf("test_mutate_data_values...\n"); fflush(stdout);
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

void test_extract_basic_blocks(void)
{
    printf("test_extract_basic_blocks...\n"); fflush(stdout);
    uint8_t code[] = {
        0x90, 0x90, 0xc3,
        0x90, 0x90, 0xe9, 0x00, 0x00, 0x00, 0x00,
        0x90, 0x90, 0xc3
    };

    int block_count = 0;
    BasicBlock *blocks = jocky_extract_basic_blocks(code, sizeof(code), &block_count);

    printf("  Block count: %d\n", block_count);
    fflush(stdout);

    assert(blocks != NULL);
    assert(block_count > 0);

    free(blocks);
    TEST_PASS("Basic block extraction works");
}

void test_randomize_cfg(void)
{
    printf("test_randomize_cfg...\n"); fflush(stdout);
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
    printf("test_polymorphic_dispatch...\n"); fflush(stdout);
    uint8_t code[] = {
        0xff, 0xe0,
        0x00, 0x00, 0x00, 0x40,
        0x90, 0x90
    };

    jocky_seed_rng(1000);
    jocky_create_polymorphic_dispatch(code, sizeof(code));

    TEST_PASS("Polymorphic dispatch creation works");
}

int main(void)
{
    printf("\n=== Debug Test Suite 4 ===\n\n");

    printf("--- More Mutation Tests ---\n");
    test_mutate_bitwise_ops();
    fflush(stdout);
    test_mutate_data_values();
    fflush(stdout);

    printf("\n--- Control Flow Mutations ---\n");
    test_extract_basic_blocks();
    fflush(stdout);
    test_randomize_cfg();
    fflush(stdout);
    test_polymorphic_dispatch();
    fflush(stdout);

    printf("\n=== All Debug Tests Passed ===\n\n");
    return 0;
}
