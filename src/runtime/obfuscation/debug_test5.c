/*
 * Debug test - final batch
 */

#include "mutation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TEST_PASS(name) printf("[PASS] %s\n", name)

void test_mutation_init(void)
{
    printf("test_mutation_init...\n"); fflush(stdout);
    MutationEngine engine;
    jocky_mutation_init(&engine, 42, 3);

    assert(engine.seed == 42);
    assert(engine.intensity == 3);
    assert(engine.pass_count > 0);
    assert(engine.passes != NULL);

    printf("  Engine initialized OK\n");
    fflush(stdout);

    TEST_PASS("Mutation engine initialization works");
}

void test_polymorphic_mutations(void)
{
    printf("test_polymorphic_mutations...\n"); fflush(stdout);
    uint8_t code[128];
    memset(code, 0x90, sizeof(code));

    jocky_seed_rng(1200);
    MutationStats before = jocky_get_mutation_stats();
    printf("  Before stats: %d mutations\n", before.mutations_applied);
    fflush(stdout);

    jocky_apply_polymorphic_mutations(code, sizeof(code));
    printf("  Applied polymorphic mutations\n");
    fflush(stdout);

    MutationStats after = jocky_get_mutation_stats();
    printf("  After stats: %d mutations\n", after.mutations_applied);
    fflush(stdout);

    assert(after.mutations_applied >= before.mutations_applied);
    TEST_PASS("Polymorphic mutations apply successfully");
}

void test_mutation_enable_disable(void)
{
    printf("test_mutation_enable_disable...\n"); fflush(stdout);
    uint8_t code[64];
    memset(code, 0x90, sizeof(code));

    jocky_set_mutation_enabled(false);
    assert(!jocky_is_mutation_enabled());
    printf("  Mutations disabled\n");
    fflush(stdout);

    jocky_reset_mutation_stats();
    jocky_apply_polymorphic_mutations(code, sizeof(code));
    MutationStats disabled_stats = jocky_get_mutation_stats();
    printf("  Disabled stats: %d\n", disabled_stats.mutations_applied);
    fflush(stdout);

    jocky_set_mutation_enabled(true);
    assert(jocky_is_mutation_enabled());
    printf("  Mutations enabled\n");
    fflush(stdout);

    memset(code, 0x90, sizeof(code));
    jocky_reset_mutation_stats();
    jocky_apply_polymorphic_mutations(code, sizeof(code));
    MutationStats enabled_stats = jocky_get_mutation_stats();
    printf("  Enabled stats: %d\n", enabled_stats.mutations_applied);
    fflush(stdout);

    assert(disabled_stats.mutations_applied < enabled_stats.mutations_applied);

    TEST_PASS("Mutation enable/disable control works");
}

int main(void)
{
    printf("\n=== Debug Test Suite 5 ===\n\n");

    printf("--- Engine Management ---\n");
    test_mutation_init();
    fflush(stdout);

    printf("\n--- Polymorphic Mutations ---\n");
    test_polymorphic_mutations();
    fflush(stdout);

    printf("\n--- Mutation Control ---\n");
    test_mutation_enable_disable();
    fflush(stdout);

    printf("\n=== All Debug Tests Passed ===\n\n");
    return 0;
}
