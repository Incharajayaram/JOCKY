/*
 * Polymorphic Obfuscation: Practical Examples
 *
 * Demonstrates real-world usage patterns for runtime code mutation
 * and self-modifying code techniques.
 */

#include "mutation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================================
 * Example 1: Basic Runtime Mutation
 * ============================================================================ */

void example_basic_mutation(void)
{
    printf("\n=== Example 1: Basic Runtime Mutation ===\n");

    uint8_t code[] = {
        0x55,                       /* push rbp */
        0x48, 0x89, 0xe5,          /* mov rbp, rsp */
        0x48, 0x01, 0xc1,          /* add rcx, rax */
        0x48, 0x31, 0xc0,          /* xor rax, rax */
        0x90,                       /* nop */
        0xc3                        /* ret */
    };

    printf("Original code size: %zu bytes\n", sizeof(code));
    printf("Original bytes: ");
    for (size_t i = 0; i < sizeof(code); i++) {
        printf("%02x ", code[i]);
    }
    printf("\n");

    MutationEngine engine;
    jocky_mutation_init(&engine, 42, 2);

    printf("\nApplying mutations (intensity=2)...\n");
    jocky_apply_polymorphic_mutations(code, sizeof(code));

    printf("Mutated bytes: ");
    for (size_t i = 0; i < sizeof(code); i++) {
        printf("%02x ", code[i]);
    }
    printf("\n");

    MutationStats stats = jocky_get_mutation_stats();
    printf("\nMutation statistics:\n");
    printf("  Total mutations applied: %d\n", stats.mutations_applied);
    printf("  Instructions modified: %d\n", stats.instructions_modified);
    printf("  Blocks reordered: %d\n", stats.blocks_reordered);

    free(engine.passes);
}

/* ============================================================================
 * Example 2: Multi-Pass Mutation with Different Intensities
 * ============================================================================ */

void example_intensity_comparison(void)
{
    printf("\n=== Example 2: Intensity Level Comparison ===\n");

    uint8_t original_code[] = {
        0x48, 0x8b, 0x45, 0xf8,    /* mov rax, [rbp-8] */
        0x48, 0x83, 0xc0, 0x01,    /* add rax, 1 */
        0x48, 0x89, 0x45, 0xf0,    /* mov [rbp-16], rax */
        0x48, 0x8b, 0x45, 0xf0,    /* mov rax, [rbp-16] */
        0xc3                        /* ret */
    };

    for (int intensity = 1; intensity <= 5; intensity++) {
        uint8_t code[64];
        memcpy(code, original_code, sizeof(original_code));

        jocky_seed_rng(intensity * 100);
        jocky_reset_mutation_stats();

        printf("\n  Intensity level %d:\n", intensity);
        jocky_apply_mutations_intensity(code, sizeof(original_code), intensity);

        MutationStats stats = jocky_get_mutation_stats();
        printf("    Mutations applied: %d\n", stats.mutations_applied);
        printf("    Instructions modified: %d\n", stats.instructions_modified);

        int changes = 0;
        for (size_t i = 0; i < sizeof(original_code); i++) {
            if (code[i] != original_code[i]) changes++;
        }
        printf("    Bytes changed: %d/%zu (%.1f%%)\n",
               changes, sizeof(original_code),
               100.0 * changes / sizeof(original_code));
    }
}

/* ============================================================================
 * Example 3: Self-Modifying Code with Checksums
 * ============================================================================ */

void example_self_modifying_code(void)
{
    printf("\n=== Example 3: Self-Modifying Code with Checksums ===\n");

    uint8_t original[] = {
        0x55, 0x48, 0x89, 0xe5, 0xc3
    };

    uint8_t mutated[] = {
        0x55, 0x48, 0x8d, 0x2c, 0xc3
    };

    uint32_t checksum = jocky_compute_code_checksum(mutated, sizeof(mutated));
    printf("Original code checksum: 0x%08x\n",
           jocky_compute_code_checksum(original, sizeof(original)));
    printf("Mutated code checksum: 0x%08x\n", checksum);

    SelfModifyingSegment segment = {
        .address = (uintptr_t)original,
        .mutation_payload = mutated,
        .payload_len = sizeof(mutated),
        .checksum = checksum,
        .mutation_count = 0
    };

    printf("\nBefore modification:\n");
    printf("  Segment address: 0x%lx\n", segment.address);
    printf("  Mutation count: %d\n", segment.mutation_count);
    printf("  Integrity: %s\n",
           jocky_verify_code_integrity(&segment) ? "Valid" : "Invalid");

    printf("\nApplying self-modification...\n");
    jocky_apply_self_mutations(&segment);

    printf("\nAfter modification:\n");
    printf("  Mutation count: %d\n", segment.mutation_count);
    printf("  Integrity: %s\n",
           jocky_verify_code_integrity(&segment) ? "Valid" : "Invalid");
}

/* ============================================================================
 * Example 4: Polymorphic Dispatch Creation
 * ============================================================================ */

void example_polymorphic_dispatch(void)
{
    printf("\n=== Example 4: Polymorphic Dispatch ===\n");

    uint8_t code[32];
    memset(code, 0x90, sizeof(code));

    code[0] = 0xff;
    code[1] = 0xe0;
    *(uint32_t *)(code + 2) = 0x40000000;

    printf("Original jump target: 0x40000000\n");
    printf("Original bytes: %02x %02x %02x %02x %02x %02x\n",
           code[0], code[1], code[2], code[3], code[4], code[5]);

    jocky_seed_rng(9999);
    jocky_create_polymorphic_dispatch(code, sizeof(code));

    printf("\nAfter polymorphic dispatch:\n");
    printf("Mutated bytes: %02x %02x %02x %02x %02x %02x\n",
           code[0], code[1], code[2], code[3], code[4], code[5]);
    printf("Target is now XOR-obfuscated with random key\n");
}

/* ============================================================================
 * Example 5: Control Flow Randomization
 * ============================================================================ */

void example_control_flow_randomization(void)
{
    printf("\n=== Example 5: Control Flow Randomization ===\n");

    uint8_t code[64];
    memset(code, 0x90, sizeof(code));

    code[4] = 0xc3;
    code[12] = 0xe9;
    code[20] = 0xc3;
    code[28] = 0xc3;

    int block_count = 0;
    BasicBlock *blocks = jocky_extract_basic_blocks(code, sizeof(code), &block_count);

    printf("Extracted %d basic blocks:\n", block_count);
    for (int i = 0; i < block_count; i++) {
        printf("  Block %d: 0x%lx -> 0x%lx\n",
               i, blocks[i].start, blocks[i].end);
    }

    printf("\nRandomizing control flow...\n");
    jocky_seed_rng(8888);
    jocky_randomize_cfg(code, sizeof(code));

    printf("CFG randomization complete\n");

    MutationStats stats = jocky_get_mutation_stats();
    printf("Blocks reordered: %d\n", stats.blocks_reordered);

    free(blocks);
}

/* ============================================================================
 * Example 6: Statistical Analysis of Mutations
 * ============================================================================ */

void example_statistical_analysis(void)
{
    printf("\n=== Example 6: Statistical Analysis ===\n");

    printf("Running 1000 mutations on random code...\n");

    jocky_reset_mutation_stats();

    for (int run = 0; run < 1000; run++) {
        uint8_t code[256];
        for (int i = 0; i < 256; i++) {
            code[i] = rand() & 0xFF;
        }

        jocky_apply_mutations_intensity(code, sizeof(code), 3);
    }

    MutationStats final_stats = jocky_get_mutation_stats();

    printf("\nCumulative statistics:\n");
    printf("  Total mutations: %d\n", final_stats.mutations_applied);
    printf("  Avg per run: %.2f\n",
           (float)final_stats.mutations_applied / 1000.0);
    printf("  Instructions modified: %d\n", final_stats.instructions_modified);
    printf("  Avg instructions per run: %.2f\n",
           (float)final_stats.instructions_modified / 1000.0);
    printf("  Blocks reordered: %d\n", final_stats.blocks_reordered);
}

/* ============================================================================
 * Example 7: Mutation Control & Toggling
 * ============================================================================ */

void example_mutation_control(void)
{
    printf("\n=== Example 7: Mutation Control ===\n");

    uint8_t code[64];
    memset(code, 0x90, sizeof(code));

    printf("Testing mutation enable/disable:\n\n");

    printf("Step 1: Mutations ENABLED\n");
    jocky_set_mutation_enabled(true);
    jocky_reset_mutation_stats();
    jocky_apply_polymorphic_mutations(code, sizeof(code));
    MutationStats enabled = jocky_get_mutation_stats();
    printf("  Mutations applied: %d\n", enabled.mutations_applied);

    printf("\nStep 2: Mutations DISABLED\n");
    memset(code, 0x90, sizeof(code));
    jocky_set_mutation_enabled(false);
    jocky_reset_mutation_stats();
    jocky_apply_polymorphic_mutations(code, sizeof(code));
    MutationStats disabled = jocky_get_mutation_stats();
    printf("  Mutations applied: %d\n", disabled.mutations_applied);

    printf("\nStep 3: Mutations ENABLED again\n");
    memset(code, 0x90, sizeof(code));
    jocky_set_mutation_enabled(true);
    jocky_reset_mutation_stats();
    jocky_apply_polymorphic_mutations(code, sizeof(code));
    MutationStats re_enabled = jocky_get_mutation_stats();
    printf("  Mutations applied: %d\n", re_enabled.mutations_applied);

    printf("\nConclusion: Disabling allows bypass of mutations\n");
}

/* ============================================================================
 * Example 8: Reproducible Mutations with Seeding
 * ============================================================================ */

void example_seeded_mutations(void)
{
    printf("\n=== Example 8: Reproducible Mutations ===\n");

    uint8_t code1[64];
    uint8_t code2[64];
    uint8_t code3[64];

    memset(code1, 0x90, sizeof(code1));
    memset(code2, 0x90, sizeof(code2));
    memset(code3, 0x90, sizeof(code3));

    printf("Running mutations with same seed (42)...\n");
    jocky_seed_rng(42);
    jocky_apply_polymorphic_mutations(code1, sizeof(code1));

    jocky_seed_rng(42);
    jocky_apply_polymorphic_mutations(code2, sizeof(code2));

    int match_12 = memcmp(code1, code2, sizeof(code1)) == 0 ? 1 : 0;
    printf("Results match: %s\n", match_12 ? "YES" : "NO");

    printf("\nRunning mutation with different seed (999)...\n");
    jocky_seed_rng(999);
    jocky_apply_polymorphic_mutations(code3, sizeof(code3));

    int match_13 = memcmp(code1, code3, sizeof(code1)) == 0 ? 1 : 0;
    printf("Results match original: %s\n", match_13 ? "YES" : "NO");
}

/* ============================================================================
 * Main Example Runner
 * ============================================================================ */

int main(void)
{
    printf("========================================\n");
    printf("Polymorphic Obfuscation: Usage Examples\n");
    printf("========================================\n");

    example_basic_mutation();
    example_intensity_comparison();
    example_self_modifying_code();
    example_polymorphic_dispatch();
    example_control_flow_randomization();
    example_statistical_analysis();
    example_mutation_control();
    example_seeded_mutations();

    printf("\n========================================\n");
    printf("All examples completed successfully\n");
    printf("========================================\n\n");

    return 0;
}
