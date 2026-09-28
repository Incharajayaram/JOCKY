#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/runtime/include/jocky_byovd.h"

static int test_count = 0;
static int test_passed = 0;

#define TEST(name) do { printf("[TEST] %s... ", name); test_count++; } while(0)
#define PASS() do { printf("PASS\n"); test_passed++; } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); } while(0)

void test_byovd_get_os_version(void)
{
    TEST("BYOVD: Get OS version");
    int version = jocky_byovd_get_os_version();

    if (version >= 10 && version <= 20) {
        PASS();
    } else {
        FAIL("Version should be 10-20");
    }
}

void test_byovd_select_best_driver_single(void)
{
    TEST("BYOVD: Select best driver (single)");

    JOCKY_DRIVER_MANIFEST manifest;
    memset(&manifest, 0, sizeof(manifest));
    strcpy(manifest.name, "test_driver");
    strcpy(manifest.filename, "/drivers/test.sys");
    manifest.target_os_min = 10;
    manifest.target_os_max = 19;
    manifest.exploit_reliability = 0.95f;

    JOCKY_DRIVER_MANIFEST selected;
    int result = jocky_byovd_select_best_driver(&manifest, 1, &selected);

    if (result == 0 && strcmp(selected.name, "test_driver") == 0) {
        PASS();
    } else {
        FAIL("Should select available driver");
    }
}

void test_byovd_select_best_driver_multiple(void)
{
    TEST("BYOVD: Select best driver (multiple)");

    JOCKY_DRIVER_MANIFEST manifests[3];

    memset(&manifests[0], 0, sizeof(JOCKY_DRIVER_MANIFEST));
    strcpy(manifests[0].name, "driver1");
    manifests[0].target_os_min = 10;
    manifests[0].target_os_max = 19;
    manifests[0].exploit_reliability = 0.5f;

    memset(&manifests[1], 0, sizeof(JOCKY_DRIVER_MANIFEST));
    strcpy(manifests[1].name, "driver2");
    manifests[1].target_os_min = 10;
    manifests[1].target_os_max = 19;
    manifests[1].exploit_reliability = 0.95f;

    memset(&manifests[2], 0, sizeof(JOCKY_DRIVER_MANIFEST));
    strcpy(manifests[2].name, "driver3");
    manifests[2].target_os_min = 15;
    manifests[2].target_os_max = 19;
    manifests[2].exploit_reliability = 0.99f;

    JOCKY_DRIVER_MANIFEST selected;
    int result = jocky_byovd_select_best_driver(manifests, 3, &selected);

    int current_os = jocky_byovd_get_os_version();
    if (result == 0 && current_os >= selected.target_os_min && current_os <= selected.target_os_max) {
        PASS();
    } else {
        FAIL("Should select compatible driver with highest reliability");
    }
}

void test_byovd_select_best_driver_no_match(void)
{
    TEST("BYOVD: Select best driver (no match)");

    JOCKY_DRIVER_MANIFEST manifest;
    memset(&manifest, 0, sizeof(manifest));
    strcpy(manifest.name, "future_driver");
    manifest.target_os_min = 50;  /* Way in the future */
    manifest.target_os_max = 60;
    manifest.exploit_reliability = 0.99f;

    JOCKY_DRIVER_MANIFEST selected;
    int result = jocky_byovd_select_best_driver(&manifest, 1, &selected);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject incompatible driver");
    }
}

void test_byovd_select_best_driver_null_inputs(void)
{
    TEST("BYOVD: Select best driver (NULL inputs)");

    JOCKY_DRIVER_MANIFEST selected;
    int result = jocky_byovd_select_best_driver(NULL, 0, &selected);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL or zero manifests");
    }
}

void test_byovd_load_from_manifest(void)
{
    TEST("BYOVD: Load driver from manifest");

    JOCKY_DRIVER_MANIFEST manifest;
    memset(&manifest, 0, sizeof(manifest));
    strcpy(manifest.filename, "test.sys");
    manifest.target_os_min = 10;
    manifest.target_os_max = 19;

    int handle;
    int result = jocky_byovd_load_from_manifest(&manifest, &handle);

    #ifdef _WIN32
    if (result == 0 && handle != -1) {
        PASS();
        jocky_byovd_unload(handle);
    } else {
        FAIL("Should load driver on Windows");
    }
    #else
    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject on non-Windows");
    }
    #endif
}

void test_byovd_query_status_valid(void)
{
    TEST("BYOVD: Query valid driver status");

    int loaded = 0;
    char version[32] = {0};
    int result = jocky_byovd_query_status(1, &loaded, version);

    if (result == 0 && loaded == 1 && strlen(version) > 0) {
        PASS();
    } else {
        FAIL("Should query valid status");
    }
}

void test_byovd_query_status_invalid(void)
{
    TEST("BYOVD: Query invalid driver status");

    int loaded = 0;
    int result = jocky_byovd_query_status(-1, &loaded, NULL);

    if (result == 0 && loaded == 0) {
        PASS();
    } else {
        FAIL("Should report unloaded for invalid handle");
    }
}

void test_byovd_test_exploit_valid(void)
{
    TEST("BYOVD: Test exploit (valid handle)");

    int success = 0;
    int result = jocky_byovd_test_exploit(1, &success);

    if (result == 0) {
        PASS();
    } else {
        FAIL("Should test exploit");
    }
}

void test_byovd_test_exploit_invalid(void)
{
    TEST("BYOVD: Test exploit (invalid handle)");

    int success = 1;
    int result = jocky_byovd_test_exploit(-1, &success);

    if (result == 0 && success == 0) {
        PASS();
    } else {
        FAIL("Should fail exploit on invalid handle");
    }
}

int main(void)
{
    printf("=== JOCKY BYOVD Driver Selection Test Suite ===\n\n");

    test_byovd_get_os_version();
    test_byovd_select_best_driver_single();
    test_byovd_select_best_driver_multiple();
    test_byovd_select_best_driver_no_match();
    test_byovd_select_best_driver_null_inputs();
    test_byovd_load_from_manifest();
    test_byovd_query_status_valid();
    test_byovd_query_status_invalid();
    test_byovd_test_exploit_valid();
    test_byovd_test_exploit_invalid();

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);

    return (test_passed == test_count) ? 0 : 1;
}
