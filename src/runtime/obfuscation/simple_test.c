/*
 * Simple test to debug the polymorphic obfuscation system
 */

#include "mutation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    printf("Test 1: Basic initialization\n");
    jocky_seed_rng(42);
    printf("  RNG seeded\n");

    uint32_t val1 = jocky_random_u32();
    printf("  Random value: 0x%x\n", val1);

    printf("\nTest 2: Instruction mutation\n");
    uint8_t code[] = {0x90, 0x90, 0x90, 0x90};
    jocky_mutate_instructions(code, sizeof(code));
    printf("  Instruction mutation done\n");

    printf("\nTest 3: Polymorphic mutations with intensity 1\n");
    uint8_t code2[] = {0x90, 0x90, 0x90, 0x90};
    jocky_apply_mutations_intensity(code2, sizeof(code2), 1);
    printf("  Polymorphic mutations done\n");

    MutationStats stats = jocky_get_mutation_stats();
    printf("  Stats: %d mutations\n", stats.mutations_applied);

    printf("\nTest 4: Reshuffle mutations\n");
    jocky_reshuffle_mutations();
    printf("  Shuffle done\n");

    printf("\nAll tests passed!\n");
    return 0;
}
