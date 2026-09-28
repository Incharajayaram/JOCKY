/*
 * Polymorphic Obfuscation: Runtime Mutation and Randomization
 *
 * Enables runtime mutation of obfuscation passes with:
 * - Randomized pass selection and ordering
 * - Instruction-level mutations
 * - Control flow randomization
 * - Self-modifying code capabilities
 *
 * Portable across Windows x86-64 and Linux x86-64.
 */

#ifndef JOCKY_MUTATION_H
#define JOCKY_MUTATION_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Mutation Pass Registry & Execution
 * ============================================================================ */

typedef void (*mutation_pass_fn)(uint8_t *code, size_t len);

typedef struct {
    const char *name;
    mutation_pass_fn apply;
    int priority;
    int weight;
} MutationPass;

typedef struct {
    MutationPass *passes;
    int pass_count;
    uint32_t seed;
    int intensity;
} MutationEngine;

/* Core polymorphic mutation API */

/* Initialize the mutation engine with optional seed (0 = random) */
void jocky_mutation_init(MutationEngine *engine, uint32_t seed, int intensity);

/* Apply random subset of mutations to code buffer */
void jocky_apply_polymorphic_mutations(uint8_t *code, size_t len);

/* Apply mutations with specific intensity level (1-5) */
void jocky_apply_mutations_intensity(uint8_t *code, size_t len, int intensity);

/* Shuffle pass order for next execution */
void jocky_reshuffle_mutations(void);

/* ============================================================================
 * Instruction-Level Mutations
 * ============================================================================ */

/* Substitute equivalent instructions (ADD r1, r2 -> LEA r1, [r1+r2], etc) */
void jocky_mutate_instructions(uint8_t *code, size_t len);

/* Randomize immediates: add offsets + compensation instructions */
void jocky_mutate_constants(uint8_t *code, size_t len);

/* Inject junk/noise instructions to break pattern matching */
void jocky_inject_junk_code(uint8_t *code, size_t len);

/* Apply De Morgan's law substitutions for bitwise operations */
void jocky_mutate_bitwise_ops(uint8_t *code, size_t len);

/* Register mutations for data values (constants, predicates) */
void jocky_mutate_data_values(uint8_t *code, size_t len);

/* ============================================================================
 * Control Flow Mutations
 * ============================================================================ */

typedef struct {
    uintptr_t start;
    uintptr_t end;
    uintptr_t fallthrough;
} BasicBlock;

/* Extract basic blocks from code buffer */
BasicBlock *jocky_extract_basic_blocks(uint8_t *code, size_t len, int *out_count);

/* Randomize basic block execution order */
void jocky_randomize_cfg(uint8_t *code, size_t len);

/* Create polymorphic dispatch tables with XOR-obfuscated addresses */
void jocky_create_polymorphic_dispatch(uint8_t *code, size_t len);

/* Reorder switch cases to defeat pattern analysis */
void jocky_randomize_switch_cases(uint8_t *code, size_t len);

/* ============================================================================
 * Self-Modifying Code
 * ============================================================================ */

typedef struct {
    uintptr_t address;
    uint8_t *mutation_payload;
    size_t payload_len;
    int mutation_count;
    uint32_t checksum;
} SelfModifyingSegment;

/* Enable writable code sections for self-modification */
void jocky_enable_code_mutation(uintptr_t code_addr, size_t code_len);

/* Apply self-modifying mutations to writable code section */
void jocky_apply_self_mutations(SelfModifyingSegment *segment);

/* Periodically re-mutate code (requires embedded mutation payloads) */
void jocky_enable_periodic_mutation(int interval_seconds);

/* Compute checksum to detect external tampering */
uint32_t jocky_compute_code_checksum(uint8_t *code, size_t len);

/* Verify code integrity after mutation */
bool jocky_verify_code_integrity(SelfModifyingSegment *segment);

/* ============================================================================
 * Randomization Utilities
 * ============================================================================ */

/* Seed global RNG (0 = use entropy) */
void jocky_seed_rng(uint32_t seed);

/* Generate random u32 in range [min, max) */
uint32_t jocky_random_range(uint32_t min, uint32_t max);

/* Generate random u32 */
uint32_t jocky_random_u32(void);

/* Generate random u64 */
uint64_t jocky_random_u64(void);

/* Shuffle array in-place */
void jocky_shuffle_array(void *array, size_t count, size_t elem_size);

/* ============================================================================
 * Mutation Statistics & Control
 * ============================================================================ */

typedef struct {
    uint32_t mutations_applied;
    uint32_t instructions_modified;
    uint32_t blocks_reordered;
    uint32_t self_modifications;
    uint64_t total_cycles;
} MutationStats;

/* Get current mutation statistics */
MutationStats jocky_get_mutation_stats(void);

/* Reset mutation statistics */
void jocky_reset_mutation_stats(void);

/* Enable/disable mutation globally */
void jocky_set_mutation_enabled(bool enabled);

/* Query if mutations are active */
bool jocky_is_mutation_enabled(void);

#ifdef __cplusplus
}
#endif

#endif /* JOCKY_MUTATION_H */
