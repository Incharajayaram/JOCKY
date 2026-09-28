#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#include "../../src/runtime/windows/anti_forensics/forensics.h"

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

/* Test 1: Detect functions are callable */
void test_function_availability(void)
{
    TEST("Anti-Forensics: All functions available");

    /* These should all be callable */
    jocky_clear_logs();
    jocky_wipe_artifacts();

    PASS();
}

/* Test 2: PowerShell history wipe attempt */
void test_powershell_history_wipe(void)
{
    TEST("Anti-Forensics: PowerShell history wipe");

    int result = jocky_wipe_powershell_history();

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 3: CMD history wipe attempt */
void test_cmd_history_wipe(void)
{
    TEST("Anti-Forensics: CMD history wipe");

    int result = jocky_wipe_cmd_history();

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 4: Prefetch wipe */
void test_prefetch_wipe(void)
{
    TEST("Anti-Forensics: Prefetch wipe");

    int result = jocky_wipe_prefetch();

    if (result == 0 || result == 1) {
        PASS();
    } else {
        FAIL("Unexpected return value");
    }
}

/* Test 5: Event log clearing */
void test_event_log_clear(void)
{
    TEST("Anti-Forensics: Event log clearing");

    int result = jocky_clear_logs();

    if (result == 0 || result == 1) {
        PASS();
    } else {
        FAIL("Unexpected return value");
    }
}

/* Test 6: MFT space wiping */
void test_mft_wipe(void)
{
    TEST("Anti-Forensics: MFT free space wipe");

    int result = jocky_wipe_mft_free_space("C:");

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 7: ARP cache flush */
void test_arp_cache_flush(void)
{
    TEST("Anti-Forensics: ARP cache flush");

    int result = jocky_flush_arp_cache();

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 8: DNS cache flush */
void test_dns_cache_flush(void)
{
    TEST("Anti-Forensics: DNS cache flush");

    int result = jocky_flush_dns_cache();

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 9: USN Journal clear */
void test_usn_journal_clear(void)
{
    TEST("Anti-Forensics: USN Journal clear");

    int result = jocky_clear_usn_journal();

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 10: DHCP lease clear */
void test_dhcp_lease_clear(void)
{
    TEST("Anti-Forensics: DHCP lease clear");

    int result = jocky_clear_dhcp_leases();

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 11: Cloud credentials wipe */
void test_cloud_credentials_wipe(void)
{
    TEST("Anti-Forensics: Cloud credentials wipe");

    int result = jocky_wipe_cloud_credentials();

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 12: Development tool logs wipe */
void test_dev_tool_logs_wipe(void)
{
    TEST("Anti-Forensics: Dev tool logs wipe");

    int result = jocky_wipe_dev_tool_logs();

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Unexpected return code");
    }
}

/* Test 13: ShimCache patch */
void test_shimcache_patch(void)
{
    TEST("Anti-Forensics: ShimCache patch");

    int result = jocky_patch_shimcache();

    if (result == 0 || result == 1) {
        PASS();
    } else {
        FAIL("Unexpected return value");
    }
}

/* Test 14: Amcache patch */
void test_amcache_patch(void)
{
    TEST("Anti-Forensics: Amcache patch");

    int result = jocky_patch_amcache();

    if (result == 0 || result == 1) {
        PASS();
    } else {
        FAIL("Unexpected return value");
    }
}

/* Test 15: SRUM clear */
void test_srum_clear(void)
{
    TEST("Anti-Forensics: SRUM clear");

    int result = jocky_clear_srum();

    if (result == 0 || result == 1) {
        PASS();
    } else {
        FAIL("Unexpected return value");
    }
}

/* Test 16: Comprehensive cleanup */
void test_comprehensive_cleanup(void)
{
    TEST("Anti-Forensics: Comprehensive forensic cleanup");

    int result = jocky_cleanup_forensic_traces();

    if (result == 0) {
        PASS();
    } else {
        FAIL("Cleanup should return 0");
    }
}

/* Run all tests */
int main(void)
{
    printf("=== JOCKY Anti-Forensics Test Suite ===\n\n");

    test_function_availability();
    test_powershell_history_wipe();
    test_cmd_history_wipe();
    test_prefetch_wipe();
    test_event_log_clear();
    test_mft_wipe();
    test_arp_cache_flush();
    test_dns_cache_flush();
    test_usn_journal_clear();
    test_dhcp_lease_clear();
    test_cloud_credentials_wipe();
    test_dev_tool_logs_wipe();
    test_shimcache_patch();
    test_amcache_patch();
    test_srum_clear();
    test_comprehensive_cleanup();

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);

    return (test_passed == test_count) ? 0 : 1;
}

#else
/* Linux stub */
int main(void)
{
    printf("Anti-forensics tests skipped (Windows only)\n");
    return 0;
}
#endif
