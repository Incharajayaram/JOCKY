#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../src/runtime/include/jocky_plugin.h"

static int test_count = 0;
static int test_passed = 0;

#define TEST(name) do { printf("[TEST] %s... ", name); test_count++; } while(0)
#define PASS() do { printf("PASS\n"); test_passed++; } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); } while(0)

void test_plugin_list_empty(void)
{
    TEST("Plugin: List empty plugins");

    JOCKY_PLUGIN* plugins;
    int count;
    int result = jocky_plugin_list(&plugins, &count);

    if (result == 0 && count == 0) {
        PASS();
    } else {
        FAIL("Should return empty list initially");
    }
}

void test_plugin_load_nonexistent(void)
{
    TEST("Plugin: Load nonexistent DLL");

    JOCKY_PLUGIN plugin;
    int result = jocky_plugin_load("/nonexistent/plugin.so", &plugin);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should fail on nonexistent file");
    }
}

void test_plugin_load_null_path(void)
{
    TEST("Plugin: Load with NULL path");

    JOCKY_PLUGIN plugin;
    int result = jocky_plugin_load(NULL, &plugin);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL path");
    }
}

void test_plugin_load_null_output(void)
{
    TEST("Plugin: Load with NULL output");

    int result = jocky_plugin_load("/tmp/plugin.so", NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL output");
    }
}

void test_plugin_run_null_plugin(void)
{
    TEST("Plugin: Run NULL plugin");

    int result = jocky_plugin_run(NULL, "args");

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL plugin");
    }
}

void test_plugin_run_no_function(void)
{
    TEST("Plugin: Run plugin without run function");

    JOCKY_PLUGIN plugin;
    memset(&plugin, 0, sizeof(plugin));
    plugin.run = NULL;

    int result = jocky_plugin_run(&plugin, "args");

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject plugin without run function");
    }
}

void test_plugin_unload_null(void)
{
    TEST("Plugin: Unload NULL plugin");

    int result = jocky_plugin_unload(NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL plugin");
    }
}

void test_plugin_unload_valid(void)
{
    TEST("Plugin: Unload valid plugin");

    JOCKY_PLUGIN plugin;
    memset(&plugin, 0, sizeof(plugin));
    strcpy(plugin.name, "test");
    plugin.shutdown = NULL;

    int result = jocky_plugin_unload(&plugin);

    if (result == 0) {
        PASS();
    } else {
        FAIL("Should unload plugin");
    }
}

void test_plugin_get_null_name(void)
{
    TEST("Plugin: Get plugin with NULL name");

    JOCKY_PLUGIN* plugin;
    int result = jocky_plugin_get(NULL, &plugin);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL name");
    }
}

void test_plugin_get_nonexistent(void)
{
    TEST("Plugin: Get nonexistent plugin");

    JOCKY_PLUGIN* plugin;
    int result = jocky_plugin_get("nonexistent", &plugin);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should return -1 for nonexistent plugin");
    }
}

void test_plugin_list_null_outputs(void)
{
    TEST("Plugin: List with NULL outputs");

    int result = jocky_plugin_list(NULL, NULL);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject NULL outputs");
    }
}

int main(void)
{
    printf("=== JOCKY Plugin System Test Suite ===\n\n");

    test_plugin_list_empty();
    test_plugin_load_nonexistent();
    test_plugin_load_null_path();
    test_plugin_load_null_output();
    test_plugin_run_null_plugin();
    test_plugin_run_no_function();
    test_plugin_unload_null();
    test_plugin_unload_valid();
    test_plugin_get_null_name();
    test_plugin_get_nonexistent();
    test_plugin_list_null_outputs();

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);

    return (test_passed == test_count) ? 0 : 1;
}
