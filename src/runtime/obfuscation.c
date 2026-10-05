#include "../include/jocky_obfuscation.h"
#include "../include/jocky_anti_analysis.h"
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <unistd.h>
#include <sys/mman.h>
#endif
#include <stdio.h>
#include <time.h>
#include <stdint.h>

static int code_mutation_enabled = 0;
static struct {
    void* original_addr;
    uint8_t* original_code;
    int original_size;
} active_patches[64];
static int active_patch_count = 0;

int jocky_enable_code_mutation(void) {
    code_mutation_enabled = 1;
    return 0;
}

int jocky_disable_code_mutation(void) {
    code_mutation_enabled = 0;
    return 0;
}

int jocky_patch_code(void* target_addr, const uint8_t* patch_code, int patch_size) {
    if (!target_addr || !patch_code || patch_size <= 0 || !code_mutation_enabled) {
        return -1;
    }

    if (jocky_make_code_writable(target_addr, patch_size) != 0) {
        return -1;
    }

    memcpy(target_addr, patch_code, patch_size);

    if (jocky_make_code_executable(target_addr, patch_size) != 0) {
        return -1;
    }

    return 0;
}

int jocky_restore_code(void* target_addr, int patch_size) {
    if (!target_addr || patch_size <= 0) {
        return -1;
    }

    for (int i = 0; i < active_patch_count; i++) {
        if (active_patches[i].original_addr == target_addr) {
            jocky_patch_code(target_addr, active_patches[i].original_code, patch_size);
            free(active_patches[i].original_code);
            active_patches[i].original_addr = NULL;
            return 0;
        }
    }

    return -1;
}

int jocky_generate_runtime_stub(const char* function_name, uint8_t* output, int* output_size) {
    if (!function_name || !output || !output_size || *output_size < 32) {
        return -1;
    }

    uint8_t stub[] = {
        0x55,
        0x48, 0x89, 0xe5,
        0xb8, 0x00, 0x00, 0x00, 0x00,
        0x5d,
        0xc3
    };

    if (*output_size < sizeof(stub)) {
        return -1;
    }

    memcpy(output, stub, sizeof(stub));
    *output_size = sizeof(stub);
    return 0;
}

int jocky_hook_function(void* original_func, void* hook_func, jocky_code_patch_t* patch_handle) {
    if (!original_func || !hook_func || !patch_handle) {
        return -1;
    }

    uint8_t* original_code = (uint8_t*)malloc(16);
    if (!original_code) {
        return -1;
    }

    memcpy(original_code, original_func, 16);

    uint8_t jmp_stub[] = {
        0x48, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xff, 0xe0
    };

    uint64_t hook_addr = (uint64_t)hook_func;
    memcpy(&jmp_stub[2], &hook_addr, 8);

    if (jocky_patch_code(original_func, jmp_stub, sizeof(jmp_stub)) != 0) {
        free(original_code);
        return -1;
    }

    if (active_patch_count >= 64) {
        free(original_code);
        return -1;
    }

    active_patches[active_patch_count].original_addr = original_func;
    active_patches[active_patch_count].original_code = original_code;
    active_patches[active_patch_count].original_size = 16;
    *patch_handle = (void*)(intptr_t)active_patch_count;
    active_patch_count++;

    return 0;
}

int jocky_unhook_function(jocky_code_patch_t patch_handle) {
    int idx = (intptr_t)patch_handle;
    if (idx < 0 || idx >= active_patch_count) {
        return -1;
    }

    return jocky_restore_code(active_patches[idx].original_addr, active_patches[idx].original_size);
}

int jocky_allocate_code_buffer(int size, void** buffer_addr) {
    if (size <= 0 || !buffer_addr) {
        return -1;
    }

    *buffer_addr = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC,
                        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (*buffer_addr == MAP_FAILED) {
        return -1;
    }

    return 0;
}

int jocky_free_code_buffer(void* buffer_addr, int size) {
    if (!buffer_addr || size <= 0) {
        return -1;
    }

    if (munmap(buffer_addr, size) < 0) {
        return -1;
    }

    return 0;
}

int jocky_make_code_executable(void* addr, int size) {
    if (!addr || size <= 0) {
        return -1;
    }

    if (mprotect(addr, size, PROT_READ | PROT_EXEC) < 0) {
        return -1;
    }

    return 0;
}

int jocky_make_code_writable(void* addr, int size) {
    if (!addr || size <= 0) {
        return -1;
    }

    if (mprotect(addr, size, PROT_READ | PROT_WRITE) < 0) {
        return -1;
    }

    return 0;
}

int jocky_mutate_based_on_analysis(void) {
    if (jocky_is_being_analyzed()) {
        if (!code_mutation_enabled) {
            jocky_enable_code_mutation();
        }
        return 1;
    }

    return 0;
}

int jocky_adaptive_obfuscation_step(void) {
    srand(time(NULL) ^ getpid());

    if (jocky_is_being_analyzed()) {
        int mutation_type = rand() % 3;

        if (mutation_type == 0) {
            jocky_enable_code_mutation();
        } else if (mutation_type == 1) {
            jocky_disable_code_mutation();
        }

        return 1;
    }

    return 0;
}
