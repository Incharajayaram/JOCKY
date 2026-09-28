#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../src/runtime/include/jocky_audit.h"

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

void test_audit_init_valid(void)
{
    TEST("Audit: Initialize with valid capacity");
    JOCKY_AUDIT_LOG log;
    int result = jocky_audit_init(&log, 100);

    if (result == 0 && log.entries != NULL && log.capacity == 100) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should initialize successfully");
    }
}

void test_audit_init_zero_capacity(void)
{
    TEST("Audit: Reject init with zero capacity");
    JOCKY_AUDIT_LOG log;
    int result = jocky_audit_init(&log, 0);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject zero capacity");
    }
}

void test_audit_init_null_log(void)
{
    TEST("Audit: Reject init with NULL log");
    int result = jocky_audit_init(NULL, 100);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL pointer");
    }
}

void test_audit_log_action_single(void)
{
    TEST("Audit: Log single action");
    JOCKY_AUDIT_LOG log;
    jocky_audit_init(&log, 100);

    int result = jocky_audit_log_action(&log, "actor1", "action1", "hash_in", "hash_out");

    if (result == 0 && log.count == 1) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should log action successfully");
    }
}

void test_audit_log_action_multiple(void)
{
    TEST("Audit: Log multiple actions");
    JOCKY_AUDIT_LOG log;
    jocky_audit_init(&log, 100);

    jocky_audit_log_action(&log, "actor1", "action1", "hash1", "hash2");
    jocky_audit_log_action(&log, "actor2", "action2", "hash3", "hash4");
    jocky_audit_log_action(&log, "actor3", "action3", "hash5", "hash6");

    if (log.count == 3) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should log 3 actions");
    }
}

void test_audit_log_action_expand_capacity(void)
{
    TEST("Audit: Auto-expand capacity");
    JOCKY_AUDIT_LOG log;
    jocky_audit_init(&log, 2);

    uint32_t initial_capacity = log.capacity;

    for (int i = 0; i < 10; i++) {
        char actor[32], action[32];
        snprintf(actor, sizeof(actor), "actor%d", i);
        snprintf(action, sizeof(action), "action%d", i);
        jocky_audit_log_action(&log, actor, action, "in", "out");
    }

    if (log.count == 10 && log.capacity > initial_capacity) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should expand capacity and log 10 actions");
    }
}

void test_audit_log_action_null_actor(void)
{
    TEST("Audit: Reject NULL actor");
    JOCKY_AUDIT_LOG log;
    jocky_audit_init(&log, 100);

    int result = jocky_audit_log_action(&log, NULL, "action", "in", "out");

    if (result == -1) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should reject NULL actor");
    }
}

void test_audit_verify_chain_empty(void)
{
    TEST("Audit: Verify empty chain");
    JOCKY_AUDIT_LOG log;
    jocky_audit_init(&log, 100);

    int valid;
    uint32_t broken_at;
    int result = jocky_audit_verify_chain(&log, &valid, &broken_at);

    if (result == 0 && valid == 1) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should verify empty chain as valid");
    }
}

void test_audit_verify_chain_single(void)
{
    TEST("Audit: Verify single entry chain");
    JOCKY_AUDIT_LOG log;
    jocky_audit_init(&log, 100);
    jocky_audit_log_action(&log, "actor", "action", "in", "out");

    int valid;
    uint32_t broken_at;
    int result = jocky_audit_verify_chain(&log, &valid, &broken_at);

    if (result == 0 && valid == 1) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should verify single entry as valid");
    }
}

void test_audit_verify_chain_multiple(void)
{
    TEST("Audit: Verify multi-entry chain integrity");
    JOCKY_AUDIT_LOG log;
    jocky_audit_init(&log, 100);

    for (int i = 0; i < 5; i++) {
        char buf[32];
        snprintf(buf, sizeof(buf), "act%d", i);
        jocky_audit_log_action(&log, buf, buf, "in", "out");
    }

    int valid;
    uint32_t broken_at;
    int result = jocky_audit_verify_chain(&log, &valid, &broken_at);

    if (result == 0 && valid == 1) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should verify 5-entry chain as valid");
    }
}

void test_audit_get_entry(void)
{
    TEST("Audit: Get entry by sequence");
    JOCKY_AUDIT_LOG log;
    jocky_audit_init(&log, 100);

    jocky_audit_log_action(&log, "actor1", "action1", "in1", "out1");
    jocky_audit_log_action(&log, "actor2", "action2", "in2", "out2");

    JOCKY_AUDIT_ENTRY* entry;
    int result = jocky_audit_get_entry(&log, 1, &entry);

    if (result == 0 && entry && strcmp(entry->actor, "actor2") == 0) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should retrieve entry 1 correctly");
    }
}

void test_audit_get_entry_out_of_bounds(void)
{
    TEST("Audit: Get entry out of bounds");
    JOCKY_AUDIT_LOG log;
    jocky_audit_init(&log, 100);
    jocky_audit_log_action(&log, "actor", "action", "in", "out");

    JOCKY_AUDIT_ENTRY* entry;
    int result = jocky_audit_get_entry(&log, 999, &entry);

    if (result == -1) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should reject out of bounds");
    }
}

void test_audit_export_import(void)
{
    TEST("Audit: Export and import log");
    JOCKY_AUDIT_LOG log1, log2;
    jocky_audit_init(&log1, 100);

    jocky_audit_log_action(&log1, "actor1", "action1", "in1", "out1");
    jocky_audit_log_action(&log1, "actor2", "action2", "in2", "out2");

    const char* filename = "/tmp/jocky_audit_test.bin";
    int export_result = jocky_audit_export(&log1, filename);
    int import_result = jocky_audit_import(&log2, filename);

    if (export_result == 0 && import_result == 0 && log2.count == 2) {
        PASS();
        jocky_audit_free(&log1);
        jocky_audit_free(&log2);
        remove(filename);
    } else {
        FAIL("Should export and import successfully");
    }
}

void test_audit_clear(void)
{
    TEST("Audit: Clear log");
    JOCKY_AUDIT_LOG log;
    jocky_audit_init(&log, 100);

    jocky_audit_log_action(&log, "actor", "action", "in", "out");
    int clear_result = jocky_audit_clear(&log);

    if (clear_result == 0 && log.count == 0) {
        PASS();
        jocky_audit_free(&log);
    } else {
        FAIL("Should clear log");
    }
}

void test_provenance_record_single(void)
{
    TEST("Provenance: Record single transformation");
    int result = jocky_provenance_record("source1", "compress", "output1");

    if (result == 0) {
        PASS();
    } else {
        FAIL("Should record provenance");
    }
}

void test_provenance_record_multiple(void)
{
    TEST("Provenance: Record multiple transformations");

    jocky_provenance_record("raw_data", "encrypt", "encrypted");
    jocky_provenance_record("encrypted", "compress", "compressed");
    jocky_provenance_record("compressed", "exfil", "exfiltrated");

    JOCKY_PROVENANCE_ENTRY* entries;
    int count;
    int result = jocky_provenance_get_chain("any", &entries, &count);

    if (result == 0 && count >= 3) {
        PASS();
    } else {
        FAIL("Should record 3+ provenance entries");
    }
}

void test_provenance_null_source(void)
{
    TEST("Provenance: Reject NULL source");
    int result = jocky_provenance_record(NULL, "transform", "output");

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL source");
    }
}

int main(void)
{
    printf("=== JOCKY Audit & Provenance Test Suite ===\n\n");

    test_audit_init_valid();
    test_audit_init_zero_capacity();
    test_audit_init_null_log();
    test_audit_log_action_single();
    test_audit_log_action_multiple();
    test_audit_log_action_expand_capacity();
    test_audit_log_action_null_actor();
    test_audit_verify_chain_empty();
    test_audit_verify_chain_single();
    test_audit_verify_chain_multiple();
    test_audit_get_entry();
    test_audit_get_entry_out_of_bounds();
    test_audit_export_import();
    test_audit_clear();
    test_provenance_record_single();
    test_provenance_record_multiple();
    test_provenance_null_source();

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);

    return (test_passed == test_count) ? 0 : 1;
}
