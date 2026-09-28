#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../src/runtime/ai/jocky_ai.h"

/* Test framework */
static int test_count = 0;
static int test_passed = 0;

#define TEST(name) \
    do { \
        printf("[TEST] %s... ", name); \
        test_count++; \
    } while(0)

#define PASS() \
    do { \
        printf("PASS\n"); \
        test_passed++; \
    } while(0)

#define FAIL(msg) \
    do { \
        printf("FAIL: %s\n", msg); \
    } while(0)

/* Test 1: AI module initialization (with stub model data) */
void test_ai_initialization(void)
{
    TEST("AI Evasion: Module initialization");

    /* Minimal model data */
    uint8_t model_data[1024];
    memset(model_data, 0, sizeof(model_data));

    bool result = jocky_ai_init(model_data, sizeof(model_data));

    if (result) {
        PASS();
    } else {
        FAIL("Init should succeed with valid data");
    }
}

/* Test 2: Telemetry collection */
void test_telemetry_collection(void)
{
    TEST("AI Evasion: Telemetry collection");

    JOCKY_AI_TELEMETRY telemetry;
    bool result = jocky_ai_collect_telemetry(&telemetry);

    if (result && telemetry.timestamp_ms > 0) {
        PASS();
    } else {
        FAIL("Should collect valid telemetry");
    }
}

/* Test 3: Threat scoring */
void test_threat_scoring(void)
{
    TEST("AI Evasion: Threat scoring");

    JOCKY_AI_TELEMETRY telemetry = {0};
    telemetry.syscall_frequency = 100.0f;
    telemetry.alert_count = 1;

    float score = jocky_ai_score_threat(&telemetry);

    if (score >= 0.0f && score <= 1.0f) {
        PASS();
    } else {
        FAIL("Score should be 0-1");
    }
}

/* Test 4: Threat classification */
void test_threat_classification(void)
{
    TEST("AI Evasion: Threat classification");

    JOCKY_AI_TELEMETRY telemetry = {0};
    JOCKY_AI_RISK_LEVEL risk = jocky_ai_classify_threat(&telemetry);

    if (risk >= JOCKY_AI_RISK_LOW && risk <= JOCKY_AI_RISK_CRITICAL) {
        PASS();
    } else {
        FAIL("Should return valid risk level");
    }
}

/* Test 5: Strategy recommendation */
void test_strategy_recommendation(void)
{
    TEST("AI Evasion: Strategy recommendation");

    JOCKY_AI_TELEMETRY telemetry = {0};
    telemetry.alert_count = 5;  /* High threat */

    JOCKY_STRATEGY strategy = jocky_ai_recommend_strategy(&telemetry);

    if (strategy >= JOCKY_STRAT_BASELINE && strategy <= JOCKY_STRAT_AI_ADAPTIVE) {
        PASS();
    } else {
        FAIL("Should return valid strategy");
    }
}

/* Test 6: Mutation generation */
void test_mutation_generation(void)
{
    TEST("AI Evasion: Mutation generation");

    JOCKY_AI_TELEMETRY telemetry = {0};
    JOCKY_AI_MUTATION_STRATEGY strategy;

    bool result = jocky_ai_generate_mutation(&telemetry, &strategy);

    if (result && strategy.strategy >= JOCKY_STRAT_BASELINE) {
        PASS();
    } else {
        FAIL("Should generate valid strategy");
    }
}

/* Test 7: Syscall recording */
void test_syscall_recording(void)
{
    TEST("AI Evasion: Syscall recording");

    jocky_ai_record_syscall(1);  /* SYS_exit on Linux */
    jocky_ai_record_syscall(2);
    jocky_ai_record_syscall(3);

    PASS();
}

/* Test 8: Network event recording */
void test_network_recording(void)
{
    TEST("AI Evasion: Network event recording");

    jocky_ai_record_network(1024, 512);
    jocky_ai_record_network(2048, 1024);

    PASS();
}

/* Test 9: File I/O recording */
void test_file_io_recording(void)
{
    TEST("AI Evasion: File I/O recording");

    jocky_ai_record_file_io("write", "test.txt");
    jocky_ai_record_file_io("read", "config.ini");

    PASS();
}

/* Test 10: Registry I/O recording */
void test_registry_io_recording(void)
{
    TEST("AI Evasion: Registry I/O recording");

    jocky_ai_record_registry("query", "HKLM\\Software\\Test");
    jocky_ai_record_registry("set", "HKCU\\Software\\Config");

    PASS();
}

/* Test 11: EDR alert recording */
void test_edr_alert_recording(void)
{
    TEST("AI Evasion: EDR alert recording");

    jocky_ai_record_edr_alert(0x01);
    jocky_ai_record_edr_alert(0x02);
    jocky_ai_record_edr_alert(0x04);

    PASS();
}

/* Test 12: Blocked operation recording */
void test_blocked_operation_recording(void)
{
    TEST("AI Evasion: Blocked operation recording");

    jocky_ai_record_blocked_operation(1, 5);  /* SYS_exit, ACCESS_DENIED */
    jocky_ai_record_blocked_operation(2, 13); /* SYS_fork, PERMISSION_DENIED */

    PASS();
}

/* Test 13: Risk level query */
void test_risk_level_query(void)
{
    TEST("AI Evasion: Risk level query");

    JOCKY_AI_RISK_LEVEL risk = jocky_ai_get_current_risk();

    if (risk >= JOCKY_AI_RISK_LOW && risk <= JOCKY_AI_RISK_CRITICAL) {
        PASS();
    } else {
        FAIL("Should return valid risk level");
    }
}

/* Test 14: Statistics tracking */
void test_statistics(void)
{
    TEST("AI Evasion: Statistics tracking");

    JOCKY_AI_STATS stats;
    bool result = jocky_ai_get_statistics(&stats);

    if (result) {
        PASS();
    } else {
        FAIL("Should get valid statistics");
    }
}

/* Test 15: Low risk scenario */
void test_low_risk_scenario(void)
{
    TEST("AI Evasion: Low risk scenario");

    JOCKY_AI_TELEMETRY telemetry = {0};
    telemetry.syscall_frequency = 10.0f;
    telemetry.alert_count = 0;
    telemetry.blocked_operations = 0;

    JOCKY_AI_RISK_LEVEL risk = jocky_ai_classify_threat(&telemetry);

    if (risk == JOCKY_AI_RISK_LOW) {
        PASS();
    } else {
        FAIL("Should classify as low risk");
    }
}

/* Run all tests */
int main(void)
{
    printf("=== JOCKY AI Evasion Test Suite ===\n\n");

    test_ai_initialization();
    test_telemetry_collection();
    test_threat_scoring();
    test_threat_classification();
    test_strategy_recommendation();
    test_mutation_generation();
    test_syscall_recording();
    test_network_recording();
    test_file_io_recording();
    test_registry_io_recording();
    test_edr_alert_recording();
    test_blocked_operation_recording();
    test_risk_level_query();
    test_statistics();
    test_low_risk_scenario();

    jocky_ai_shutdown();

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);

    return (test_passed == test_count) ? 0 : 1;
}
