/*
 * Polymorphic Obfuscation: Runtime Mutation Engine
 *
 * Implements sophisticated runtime code mutation with:
 * - Random pass selection and ordering
 * - Instruction substitution
 * - Constant mutation with compensation
 * - Control flow randomization
 * - Self-modifying code support
 */

#define _GNU_SOURCE
#include "mutation.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#endif

/* ============================================================================
 * Global State
 * ============================================================================ */

static MutationPass *g_global_passes = NULL;
static int g_global_pass_count = 0;
static uint32_t g_rng_state = 0;
static bool g_mutation_enabled = true;
static MutationStats g_mutation_stats = {0};
static bool g_engine_initialized = false;

/* ============================================================================
 * Randomization Infrastructure
 * ============================================================================ */

void jocky_seed_rng(uint32_t seed)
{
    if (seed == 0) {
        seed = (uint32_t)time(NULL) ^ getpid();
    }
    g_rng_state = seed;
}

static uint32_t xorshift32(void)
{
    uint32_t x = g_rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    g_rng_state = x;
    return x;
}

uint32_t jocky_random_u32(void)
{
    if (g_rng_state == 0) {
        jocky_seed_rng(0);
    }
    return xorshift32();
}

uint64_t jocky_random_u64(void)
{
    return ((uint64_t)jocky_random_u32() << 32) | jocky_random_u32();
}

uint32_t jocky_random_range(uint32_t min, uint32_t max)
{
    if (min >= max) return min;
    uint32_t range = max - min;
    return min + (jocky_random_u32() % range);
}

void jocky_shuffle_array(void *array, size_t count, size_t elem_size)
{
    if (count <= 1) return;

    uint8_t *arr = (uint8_t *)array;
    uint8_t *temp = malloc(elem_size);
    if (!temp) return;

    for (size_t i = count - 1; i > 0; i--) {
        size_t j = jocky_random_range(0, i + 1);

        memcpy(temp, arr + i * elem_size, elem_size);
        memcpy(arr + i * elem_size, arr + j * elem_size, elem_size);
        memcpy(arr + j * elem_size, temp, elem_size);
    }

    free(temp);
}

/* ============================================================================
 * Instruction-Level Mutations
 * ============================================================================ */

static bool is_add_instruction(uint8_t *code, size_t *instr_len)
{
    if (code[0] == 0x48 && code[1] == 0x01) {
        *instr_len = 3;
        return true;
    }
    return false;
}

static bool is_mov_instruction(uint8_t *code, size_t *instr_len)
{
    if ((code[0] & 0xf0) == 0x40 && (code[1] & 0xc0) == 0xc0) {
        *instr_len = 2;
        return true;
    }
    if (code[0] == 0x48 && (code[1] & 0xf8) == 0x88) {
        *instr_len = 3;
        return true;
    }
    return false;
}

static bool is_xor_instruction(uint8_t *code, size_t *instr_len)
{
    if (code[0] == 0x48 && code[1] == 0x31) {
        *instr_len = 3;
        return true;
    }
    if ((code[0] & 0xf0) == 0x30) {
        *instr_len = 2;
        return true;
    }
    return false;
}

void jocky_mutate_instructions(uint8_t *code, size_t len)
{
    if (!g_mutation_enabled) return;

    size_t i = 0;
    while (i < len - 3) {
        size_t instr_len = 1;

        if (is_add_instruction(code + i, &instr_len)) {
            if (jocky_random_range(0, 100) < 40) {
                code[i] = 0x48;
                code[i + 1] = 0x8d;
                g_mutation_stats.instructions_modified++;
            }
            i += instr_len;
        }
        else if (is_xor_instruction(code + i, &instr_len)) {
            if (jocky_random_range(0, 100) < 30) {
                code[i] = 0x48;
                code[i + 1] = 0x33;
                g_mutation_stats.instructions_modified++;
            }
            i += instr_len;
        }
        else {
            i++;
        }
    }
}

void jocky_mutate_constants(uint8_t *code, size_t len)
{
    if (!g_mutation_enabled) return;

    for (size_t i = 0; i < len - 4; i++) {
        if ((code[i] == 0xb8 || code[i] == 0xc7) && jocky_random_range(0, 100) < 50) {
            uint32_t const_val = *(uint32_t *)(code + i + 1);
            uint32_t offset = jocky_random_u32();

            *(uint32_t *)(code + i + 1) = const_val ^ offset;

            g_mutation_stats.mutations_applied++;
        }
    }
}

void jocky_inject_junk_code(uint8_t *code, size_t len)
{
    if (!g_mutation_enabled || len < 16) return;

    const uint8_t junk_patterns[][4] = {
        {0x50, 0x58, 0x90, 0x90},
        {0x48, 0x31, 0xc0, 0x90},
        {0x0f, 0x1f, 0x00, 0x90},
        {0x66, 0x66, 0x90, 0x90},
    };

    for (size_t i = 0; i < len - 4; i++) {
        if (jocky_random_range(0, 1000) < 5) {
            int pattern = jocky_random_range(0, 4);
            memcpy(code + i, junk_patterns[pattern], 4);
            g_mutation_stats.mutations_applied++;
        }
    }
}

void jocky_mutate_bitwise_ops(uint8_t *code, size_t len)
{
    if (!g_mutation_enabled) return;

    for (size_t i = 0; i < len - 3; i++) {
        if (code[i] == 0x48) {
            switch (code[i + 1]) {
                case 0x21:
                    if (jocky_random_range(0, 100) < 33) {
                        code[i + 1] = 0x09;
                        g_mutation_stats.instructions_modified++;
                    }
                    break;
                case 0x09:
                    if (jocky_random_range(0, 100) < 33) {
                        code[i + 1] = 0x31;
                        g_mutation_stats.instructions_modified++;
                    }
                    break;
                case 0x31:
                    if (jocky_random_range(0, 100) < 33) {
                        code[i + 1] = 0x21;
                        g_mutation_stats.instructions_modified++;
                    }
                    break;
            }
        }
    }
}

void jocky_mutate_data_values(uint8_t *code, size_t len)
{
    if (!g_mutation_enabled) return;

    for (size_t i = 0; i < len; i += 4) {
        if (jocky_random_range(0, 100) < 20) {
            uint32_t *val = (uint32_t *)(code + i);
            *val ^= jocky_random_u32();
            g_mutation_stats.mutations_applied++;
        }
    }
}

/* ============================================================================
 * Control Flow Mutations
 * ============================================================================ */

BasicBlock *jocky_extract_basic_blocks(uint8_t *code, size_t len, int *out_count)
{
    BasicBlock *blocks = malloc(sizeof(BasicBlock) * 256);
    if (!blocks) {
        *out_count = 0;
        return NULL;
    }

    int block_count = 0;
    uintptr_t block_start = (uintptr_t)code;

    for (size_t i = 0; i < len && block_count < 256; i++) {
        bool is_terminator = false;

        if (code[i] == 0xc3 || code[i] == 0xc2) {
            is_terminator = true;
        }
        else if (code[i] == 0xff && i + 1 < len && (code[i + 1] & 0xf0) == 0xe0) {
            is_terminator = true;
        }
        else if ((code[i] & 0xf0) == 0x70 || code[i] == 0xe9 || code[i] == 0xeb) {
            is_terminator = true;
        }

        if (is_terminator && block_count < 256) {
            blocks[block_count].start = block_start;
            blocks[block_count].end = (uintptr_t)(code + i + 1);
            blocks[block_count].fallthrough = blocks[block_count].end;
            block_count++;
            block_start = (uintptr_t)(code + i + 1);
        }
    }

    if (block_start < (uintptr_t)(code + len) && block_count < 256) {
        blocks[block_count].start = block_start;
        blocks[block_count].end = (uintptr_t)(code + len);
        blocks[block_count].fallthrough = blocks[block_count].end;
        block_count++;
    }

    *out_count = block_count;
    return blocks;
}

void jocky_randomize_cfg(uint8_t *code, size_t len)
{
    if (!g_mutation_enabled || len < 32) return;

    int block_count = 0;
    BasicBlock *blocks = jocky_extract_basic_blocks(code, len, &block_count);
    if (!blocks || block_count < 2) {
        free(blocks);
        return;
    }

    jocky_shuffle_array(blocks, block_count, sizeof(BasicBlock));
    g_mutation_stats.blocks_reordered += block_count;

    free(blocks);
}

void jocky_create_polymorphic_dispatch(uint8_t *code, size_t len)
{
    if (!g_mutation_enabled || len < 16) return;

    for (size_t i = 0; i < len - 8; i++) {
        if (code[i] == 0xff && (code[i + 1] & 0xf0) == 0xe0) {
            uint32_t target = *(uint32_t *)(code + i + 2);
            uint32_t key = jocky_random_u32();

            *(uint32_t *)(code + i + 2) = target ^ key;

            g_mutation_stats.mutations_applied++;
        }
    }
}

void jocky_randomize_switch_cases(uint8_t *code, size_t len)
{
    if (!g_mutation_enabled || len < 64) return;

    for (size_t i = 0; i < len - 16; i++) {
        if (code[i] == 0xff && code[i + 1] == 0x25) {
            uint32_t *table = *(uint32_t **)(code + i + 2);
            size_t table_size = jocky_random_range(4, 16);

            if ((uintptr_t)table >= (uintptr_t)code &&
                (uintptr_t)table < (uintptr_t)(code + len)) {
                jocky_shuffle_array(table, table_size, sizeof(uint32_t));
                g_mutation_stats.blocks_reordered++;
            }
        }
    }
}

/* ============================================================================
 * Self-Modifying Code
 * ============================================================================ */

void jocky_enable_code_mutation(uintptr_t code_addr, size_t code_len)
{
#ifdef _WIN32
    DWORD old_protect;
    VirtualProtect((void *)code_addr, code_len, PAGE_EXECUTE_READWRITE, &old_protect);
#else
    mprotect((void *)code_addr, code_len, PROT_READ | PROT_WRITE | PROT_EXEC);
#endif
}

void jocky_apply_self_mutations(SelfModifyingSegment *segment)
{
    if (!segment || !segment->mutation_payload) return;

    jocky_enable_code_mutation(segment->address, segment->payload_len);

    memcpy((void *)segment->address, segment->mutation_payload, segment->payload_len);
    segment->mutation_count++;

#ifdef _WIN32
    FlushInstructionCache(GetCurrentProcess(), (void *)segment->address, segment->payload_len);
    DWORD old_protect;
    VirtualProtect((void *)segment->address, segment->payload_len,
                   PAGE_EXECUTE_READ, &old_protect);
#else
    __builtin___clear_cache((void *)segment->address,
                            (void *)(segment->address + segment->payload_len));
    mprotect((void *)segment->address, segment->payload_len, PROT_READ | PROT_EXEC);
#endif

    g_mutation_stats.self_modifications++;
}

void jocky_enable_periodic_mutation(int interval_seconds)
{
    if (!g_mutation_enabled) return;

#ifdef _WIN32
    SetTimer(NULL, 0, interval_seconds * 1000, NULL);
#else
    alarm(interval_seconds);
#endif
}

uint32_t jocky_compute_code_checksum(uint8_t *code, size_t len)
{
    uint32_t checksum = 0x12345678;

    for (size_t i = 0; i < len; i++) {
        checksum = (checksum << 1) | (checksum >> 31);
        checksum ^= code[i];
    }

    return checksum;
}

bool jocky_verify_code_integrity(SelfModifyingSegment *segment)
{
    if (!segment) return false;

    uint32_t current_checksum = jocky_compute_code_checksum(
        (uint8_t *)segment->address, segment->payload_len);

    return current_checksum == segment->checksum;
}

/* ============================================================================
 * Mutation Pass Management
 * ============================================================================ */

static void apply_instruction_mutations(uint8_t *code, size_t len)
{
    jocky_mutate_instructions(code, len);
}

static void apply_constant_mutations(uint8_t *code, size_t len)
{
    jocky_mutate_constants(code, len);
}

static void apply_junk_mutations(uint8_t *code, size_t len)
{
    jocky_inject_junk_code(code, len);
}

static void apply_bitwise_mutations(uint8_t *code, size_t len)
{
    jocky_mutate_bitwise_ops(code, len);
}

static void apply_cfg_mutations(uint8_t *code, size_t len)
{
    jocky_randomize_cfg(code, len);
}

static void apply_dispatch_mutations(uint8_t *code, size_t len)
{
    jocky_create_polymorphic_dispatch(code, len);
}

static MutationPass g_available_passes[] = {
    {"instructions", apply_instruction_mutations, 2, 70},
    {"constants", apply_constant_mutations, 1, 60},
    {"junk_code", apply_junk_mutations, 1, 50},
    {"bitwise", apply_bitwise_mutations, 2, 65},
    {"control_flow", apply_cfg_mutations, 3, 75},
    {"dispatch", apply_dispatch_mutations, 2, 40},
    {NULL, NULL, 0, 0}
};

static int compare_priority(const void *a, const void *b)
{
    const MutationPass *pa = (const MutationPass *)a;
    const MutationPass *pb = (const MutationPass *)b;
    return pb->priority - pa->priority;
}

static void ensure_engine_initialized(void)
{
    if (g_engine_initialized) return;

    jocky_seed_rng(0);

    int pass_count = 0;
    for (int i = 0; g_available_passes[i].name; i++) {
        pass_count++;
    }

    if (g_global_passes) {
        free(g_global_passes);
    }

    g_global_passes = malloc(sizeof(MutationPass) * pass_count);
    if (!g_global_passes) {
        g_global_pass_count = 0;
        return;
    }

    memcpy(g_global_passes, g_available_passes, sizeof(MutationPass) * pass_count);
    g_global_pass_count = pass_count - 1;
    g_engine_initialized = true;
}

void jocky_mutation_init(MutationEngine *engine, uint32_t seed, int intensity)
{
    if (!engine) return;

    jocky_seed_rng(seed);
    ensure_engine_initialized();

    engine->intensity = (intensity < 1) ? 1 : (intensity > 5) ? 5 : intensity;
    engine->seed = seed;
    engine->passes = g_global_passes;
    engine->pass_count = g_global_pass_count;
}

void jocky_apply_polymorphic_mutations(uint8_t *code, size_t len)
{
    jocky_apply_mutations_intensity(code, len, 3);
}

void jocky_apply_mutations_intensity(uint8_t *code, size_t len, int intensity)
{
    if (!g_mutation_enabled || !code || len < 4) return;

    ensure_engine_initialized();

    if (g_global_pass_count == 0) return;

    MutationPass *passes = malloc(sizeof(MutationPass) * g_global_pass_count);
    if (!passes) return;

    memcpy(passes, g_global_passes, sizeof(MutationPass) * g_global_pass_count);

    int selected_count = 0;
    MutationPass selected[32];

    for (int i = 0; i < g_global_pass_count; i++) {
        int threshold = (100 * intensity) / 5;
        if (jocky_random_range(0, 100) < threshold && selected_count < 32) {
            selected[selected_count++] = passes[i];
        }
    }

    if (selected_count > 0) {
        qsort(selected, selected_count, sizeof(MutationPass), compare_priority);
        jocky_shuffle_array(selected, selected_count, sizeof(MutationPass));

        for (int i = 0; i < selected_count; i++) {
            if (selected[i].apply != NULL) {
                selected[i].apply(code, len);
            }
        }
    }

    free(passes);
    g_mutation_stats.mutations_applied++;
}

void jocky_reshuffle_mutations(void)
{
    ensure_engine_initialized();

    if (g_global_passes && g_global_pass_count > 0) {
        jocky_shuffle_array(g_global_passes, g_global_pass_count, sizeof(MutationPass));
    }
}

/* ============================================================================
 * Statistics & Control
 * ============================================================================ */

MutationStats jocky_get_mutation_stats(void)
{
    return g_mutation_stats;
}

void jocky_reset_mutation_stats(void)
{
    memset(&g_mutation_stats, 0, sizeof(MutationStats));
}

void jocky_set_mutation_enabled(bool enabled)
{
    g_mutation_enabled = enabled;
}

bool jocky_is_mutation_enabled(void)
{
    return g_mutation_enabled;
}
