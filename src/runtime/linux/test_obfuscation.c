#include "../include/jocky_obfuscation.h"
#include "../include/jocky_anti_analysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void test_code_mutation_enable_disable() {
    printf("Test: Code mutation enable/disable\n");

    if (jocky_enable_code_mutation() != 0) {
        printf("FAIL: Could not enable code mutation\n");
        return;
    }

    if (jocky_disable_code_mutation() != 0) {
        printf("FAIL: Could not disable code mutation\n");
        return;
    }

    printf("PASS\n");
}

void test_allocate_code_buffer() {
    printf("Test: Allocate and free code buffer\n");

    void* buffer = NULL;
    int size = 4096;

    if (jocky_allocate_code_buffer(size, &buffer) != 0) {
        printf("FAIL: Could not allocate code buffer\n");
        return;
    }

    if (!buffer) {
        printf("FAIL: Buffer pointer is null\n");
        return;
    }

    memset(buffer, 0x90, size);

    if (jocky_free_code_buffer(buffer, size) != 0) {
        printf("FAIL: Could not free code buffer\n");
        return;
    }

    printf("PASS\n");
}

void test_make_code_executable_writable() {
    printf("Test: Make code executable and writable\n");

    void* buffer = NULL;
    int size = 4096;

    if (jocky_allocate_code_buffer(size, &buffer) != 0) {
        printf("FAIL: Could not allocate buffer\n");
        return;
    }

    if (jocky_make_code_writable(buffer, size) != 0) {
        printf("FAIL: Could not make writable\n");
        jocky_free_code_buffer(buffer, size);
        return;
    }

    memset(buffer, 0xCC, size);

    if (jocky_make_code_executable(buffer, size) != 0) {
        printf("FAIL: Could not make executable\n");
        jocky_free_code_buffer(buffer, size);
        return;
    }

    jocky_free_code_buffer(buffer, size);
    printf("PASS\n");
}

void test_generate_runtime_stub() {
    printf("Test: Generate runtime stub\n");

    uint8_t stub_output[256];
    int output_size = sizeof(stub_output);

    if (jocky_generate_runtime_stub("test_func", stub_output, &output_size) != 0) {
        printf("FAIL: Could not generate stub\n");
        return;
    }

    if (output_size <= 0 || output_size >= 256) {
        printf("FAIL: Invalid stub size\n");
        return;
    }

    printf("PASS (stub size: %d bytes)\n", output_size);
}

void test_patch_and_restore() {
    printf("Test: Patch and restore code\n");

    void* buffer = NULL;
    int size = 64;

    if (jocky_allocate_code_buffer(size, &buffer) != 0) {
        printf("FAIL: Could not allocate buffer\n");
        return;
    }

    uint8_t patch[] = { 0x90, 0x90, 0x90, 0x90 };

    if (jocky_enable_code_mutation() != 0) {
        printf("FAIL: Could not enable mutation\n");
        jocky_free_code_buffer(buffer, size);
        return;
    }

    if (jocky_patch_code(buffer, patch, sizeof(patch)) != 0) {
        printf("FAIL: Could not patch code\n");
        jocky_free_code_buffer(buffer, size);
        return;
    }

    uint8_t* buf = (uint8_t*)buffer;
    if (buf[0] != 0x90 || buf[1] != 0x90) {
        printf("FAIL: Patch not applied\n");
        jocky_free_code_buffer(buffer, size);
        return;
    }

    jocky_disable_code_mutation();
    jocky_free_code_buffer(buffer, size);
    printf("PASS\n");
}

void test_null_buffer_handling() {
    printf("Test: Null buffer handling\n");

    if (jocky_allocate_code_buffer(0, NULL) == 0) {
        printf("FAIL: Should reject zero size\n");
        return;
    }

    void* buffer = NULL;
    if (jocky_allocate_code_buffer(-1, &buffer) == 0) {
        printf("FAIL: Should reject negative size\n");
        return;
    }

    if (jocky_make_code_executable(NULL, 1024) == 0) {
        printf("FAIL: Should reject null pointer\n");
        return;
    }

    if (jocky_make_code_writable(NULL, 1024) == 0) {
        printf("FAIL: Should reject null pointer\n");
        return;
    }

    printf("PASS\n");
}

void test_adaptive_obfuscation() {
    printf("Test: Adaptive obfuscation based on analysis\n");

    int result = jocky_mutate_based_on_analysis();
    if (result < 0) {
        printf("FAIL: Analysis check failed\n");
        return;
    }

    if (result == 1) {
        printf("PASS (Analysis detected, mutation enabled)\n");
    } else {
        printf("PASS (No analysis detected)\n");
    }
}

void test_adaptive_obfuscation_step() {
    printf("Test: Adaptive obfuscation step\n");

    int result = jocky_adaptive_obfuscation_step();
    if (result < 0) {
        printf("FAIL: Adaptive step failed\n");
        return;
    }

    if (result == 1) {
        printf("PASS (Analysis-adaptive step executed)\n");
    } else {
        printf("PASS (Normal step executed)\n");
    }
}

void test_runtime_stub_generation() {
    printf("Test: Multiple stub generation\n");

    uint8_t stub1[256];
    uint8_t stub2[256];
    int size1 = sizeof(stub1);
    int size2 = sizeof(stub2);

    if (jocky_generate_runtime_stub("func1", stub1, &size1) != 0) {
        printf("FAIL: First stub generation failed\n");
        return;
    }

    if (jocky_generate_runtime_stub("func2", stub2, &size2) != 0) {
        printf("FAIL: Second stub generation failed\n");
        return;
    }

    if (size1 != size2) {
        printf("FAIL: Stub sizes differ\n");
        return;
    }

    printf("PASS (generated %d-byte stubs)\n", size1);
}

void test_code_buffer_permissions() {
    printf("Test: Code buffer permission transitions\n");

    void* buffer = NULL;
    int size = 4096;

    if (jocky_allocate_code_buffer(size, &buffer) != 0) {
        printf("FAIL: Allocation failed\n");
        return;
    }

    if (jocky_make_code_writable(buffer, size) != 0) {
        printf("FAIL: Make writable failed\n");
        jocky_free_code_buffer(buffer, size);
        return;
    }

    memset(buffer, 0x55, size);

    if (jocky_make_code_executable(buffer, size) != 0) {
        printf("FAIL: Make executable failed\n");
        jocky_free_code_buffer(buffer, size);
        return;
    }

    if (jocky_make_code_writable(buffer, size) != 0) {
        printf("FAIL: Re-make writable failed\n");
        jocky_free_code_buffer(buffer, size);
        return;
    }

    jocky_free_code_buffer(buffer, size);
    printf("PASS\n");
}

int main() {
    printf("Runtime Dynamic Obfuscation Tests\n");
    printf("==================================\n\n");

    test_code_mutation_enable_disable();
    test_allocate_code_buffer();
    test_make_code_executable_writable();
    test_generate_runtime_stub();
    test_patch_and_restore();
    test_null_buffer_handling();
    test_adaptive_obfuscation();
    test_adaptive_obfuscation_step();
    test_runtime_stub_generation();
    test_code_buffer_permissions();

    printf("\n==================================\n");
    printf("All tests completed\n");

    return 0;
}
