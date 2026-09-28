#include "../include/jocky_obfuscation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_xor_obfuscate() {
    printf("Test: XOR obfuscation and deobfuscation\n");

    uint8_t original[] = "Hello, World!";
    int size = strlen((char*)original) + 1;
    uint8_t code[64];
    memcpy(code, original, size);

    uint32_t key = 0xDEADBEEF;

    if (jocky_obfuscate_code(code, size, JOCKY_XOR_OBFUSCATE, key) != 0) {
        printf("FAIL: Obfuscation failed\n");
        return;
    }

    if (memcmp(code, original, size) == 0) {
        printf("FAIL: Code not changed\n");
        return;
    }

    if (jocky_deobfuscate_code(code, size, JOCKY_XOR_OBFUSCATE, key) != 0) {
        printf("FAIL: Deobfuscation failed\n");
        return;
    }

    if (memcmp(code, original, size) != 0) {
        printf("FAIL: Deobfuscated code does not match original\n");
        return;
    }

    printf("PASS\n");
}

void test_rot_obfuscate() {
    printf("Test: ROT obfuscation and deobfuscation\n");

    uint8_t original[] = "Test Code";
    int size = strlen((char*)original) + 1;
    uint8_t code[64];
    memcpy(code, original, size);

    uint32_t key = 0x12345678;

    if (jocky_obfuscate_code(code, size, JOCKY_ROT_OBFUSCATE, key) != 0) {
        printf("FAIL: Obfuscation failed\n");
        return;
    }

    if (jocky_deobfuscate_code(code, size, JOCKY_ROT_OBFUSCATE, key) != 0) {
        printf("FAIL: Deobfuscation failed\n");
        return;
    }

    if (memcmp(code, original, size) != 0) {
        printf("FAIL: Deobfuscated code does not match original\n");
        return;
    }

    printf("PASS\n");
}

void test_shuffle_obfuscate() {
    printf("Test: SHUFFLE obfuscation and deobfuscation\n");

    uint8_t original[] = "ABCDEFGHIJ";
    int size = strlen((char*)original);
    uint8_t code[64];
    memcpy(code, original, size);

    uint32_t key = 0xABCDEF00;

    if (jocky_obfuscate_code(code, size, JOCKY_SHUFFLE_OBFUSCATE, key) != 0) {
        printf("FAIL: Obfuscation failed\n");
        return;
    }

    if (jocky_deobfuscate_code(code, size, JOCKY_SHUFFLE_OBFUSCATE, key) != 0) {
        printf("FAIL: Deobfuscation failed\n");
        return;
    }

    if (memcmp(code, original, size) != 0) {
        printf("FAIL: Deobfuscated code does not match original\n");
        return;
    }

    printf("PASS\n");
}

void test_polymorphic_mutate() {
    printf("Test: Polymorphic mutation\n");

    uint8_t original[256];
    for (int i = 0; i < 256; i++) {
        original[i] = i & 0xFF;
    }

    uint8_t output[512];
    int output_size = sizeof(output);

    if (jocky_polymorphic_mutate(original, 256, output, &output_size) != 0) {
        printf("FAIL: Polymorphic mutation failed\n");
        return;
    }

    if (output_size != 256) {
        printf("FAIL: Output size mismatch\n");
        return;
    }

    printf("PASS\n");
}

void test_inject_junk_code() {
    printf("Test: Junk code injection\n");

    uint8_t code[256];
    memset(code, 0x90, 256);

    uint8_t output[512];
    int output_size = sizeof(output);

    if (jocky_inject_junk_code(code, 256, output, &output_size) != 0) {
        printf("FAIL: Junk injection failed\n");
        return;
    }

    if (output_size <= 256) {
        printf("FAIL: Output size not increased\n");
        return;
    }

    printf("PASS (injected %d bytes)\n", output_size - 256);
}

void test_flatten_control_flow() {
    printf("Test: Control flow flattening\n");

    uint8_t code[256];
    for (int i = 0; i < 256; i++) {
        code[i] = i & 0xFF;
    }

    uint8_t output[512];
    int output_size = sizeof(output);

    if (jocky_flatten_control_flow(code, 256, output, &output_size) != 0) {
        printf("FAIL: Control flow flattening failed\n");
        return;
    }

    if (output_size != 256) {
        printf("FAIL: Output size mismatch\n");
        return;
    }

    printf("PASS\n");
}

void test_string_obfuscation() {
    printf("Test: String obfuscation\n");

    uint8_t string[] = "Secret String";
    int size = strlen((char*)string) + 1;
    uint8_t original[256];
    memcpy(original, string, size);

    uint32_t key = 0xDEADBEEF;

    if (jocky_apply_string_obfuscation(string, size, key) != 0) {
        printf("FAIL: String obfuscation failed\n");
        return;
    }

    if (memcmp(string, original, size) == 0) {
        printf("FAIL: String not changed\n");
        return;
    }

    if (jocky_apply_string_obfuscation(string, size, key) != 0) {
        printf("FAIL: String deobfuscation failed\n");
        return;
    }

    if (memcmp(string, original, size) != 0) {
        printf("FAIL: Deobfuscated string does not match\n");
        return;
    }

    printf("PASS\n");
}

void test_function_name_mangling() {
    printf("Test: Function name mangling\n");

    char mangled[256];

    if (jocky_mangle_function_name("test_function", mangled, sizeof(mangled)) != 0) {
        printf("FAIL: Function name mangling failed\n");
        return;
    }

    if (strlen(mangled) == 0 || mangled[0] != '_' || mangled[1] != 'Z') {
        printf("FAIL: Invalid mangled name\n");
        return;
    }

    if (strstr(mangled, "test_function") == NULL) {
        printf("FAIL: Original name not in mangled\n");
        return;
    }

    printf("PASS (mangled: %s)\n", mangled);
}

void test_key_generation() {
    printf("Test: Obfuscation key generation\n");

    uint32_t key1 = jocky_generate_obfuscation_key();
    uint32_t key2 = jocky_generate_obfuscation_key();

    if (key1 == 0) {
        printf("FAIL: Key 1 is zero\n");
        return;
    }

    printf("PASS (key1=0x%08X, key2=0x%08X)\n", key1, key2);
}

void test_null_inputs() {
    printf("Test: Null input handling\n");

    uint8_t code[64] = {0};
    uint8_t output[128] = {0};
    int output_size = 128;

    if (jocky_obfuscate_code(NULL, 64, JOCKY_XOR_OBFUSCATE, 0x1234) == 0) {
        printf("FAIL: Should reject null code\n");
        return;
    }

    if (jocky_polymorphic_mutate(NULL, 64, output, &output_size) == 0) {
        printf("FAIL: Should reject null code\n");
        return;
    }

    if (jocky_mangle_function_name(NULL, output, 64) == 0) {
        printf("FAIL: Should reject null name\n");
        return;
    }

    printf("PASS\n");
}

int main() {
    printf("Obfuscation Tests\n");
    printf("=================\n\n");

    test_xor_obfuscate();
    test_rot_obfuscate();
    test_shuffle_obfuscate();
    test_polymorphic_mutate();
    test_inject_junk_code();
    test_flatten_control_flow();
    test_string_obfuscation();
    test_function_name_mangling();
    test_key_generation();
    test_null_inputs();

    printf("\n=================\n");
    printf("All tests completed\n");

    return 0;
}
