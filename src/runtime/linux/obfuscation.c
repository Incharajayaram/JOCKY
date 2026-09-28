#include "../include/jocky_obfuscation.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

uint32_t jocky_generate_obfuscation_key(void) {
    srand(time(NULL) ^ getpid());
    return (uint32_t)((rand() << 16) | (rand() & 0xFFFF));
}

int jocky_obfuscate_code(uint8_t* code, int code_size, int method, uint32_t key) {
    if (!code || code_size <= 0) {
        return -1;
    }

    if (method == JOCKY_XOR_OBFUSCATE) {
        for (int i = 0; i < code_size; i++) {
            code[i] ^= (key >> (8 * (i % 4))) & 0xFF;
        }
    } else if (method == JOCKY_ROT_OBFUSCATE) {
        int rot_amount = (key % 7) + 1;
        for (int i = 0; i < code_size; i++) {
            code[i] = (code[i] << rot_amount) | (code[i] >> (8 - rot_amount));
        }
    } else if (method == JOCKY_SHUFFLE_OBFUSCATE) {
        uint8_t* temp = (uint8_t*)malloc(code_size);
        if (!temp) {
            return -1;
        }
        memcpy(temp, code, code_size);

        srand(key);
        for (int i = code_size - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            uint8_t swap = temp[i];
            temp[i] = temp[j];
            temp[j] = swap;
        }
        memcpy(code, temp, code_size);
        free(temp);
    } else {
        return -1;
    }

    return 0;
}

int jocky_deobfuscate_code(uint8_t* code, int code_size, int method, uint32_t key) {
    if (!code || code_size <= 0) {
        return -1;
    }

    if (method == JOCKY_XOR_OBFUSCATE) {
        for (int i = 0; i < code_size; i++) {
            code[i] ^= (key >> (8 * (i % 4))) & 0xFF;
        }
    } else if (method == JOCKY_ROT_OBFUSCATE) {
        int rot_amount = (key % 7) + 1;
        for (int i = 0; i < code_size; i++) {
            code[i] = (code[i] >> rot_amount) | (code[i] << (8 - rot_amount));
        }
    } else if (method == JOCKY_SHUFFLE_OBFUSCATE) {
        uint8_t* temp = (uint8_t*)malloc(code_size);
        if (!temp) {
            return -1;
        }
        memcpy(temp, code, code_size);

        srand(key);
        for (int i = code_size - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            uint8_t swap = temp[i];
            temp[i] = temp[j];
            temp[j] = swap;
        }
        memcpy(code, temp, code_size);
        free(temp);
    } else {
        return -1;
    }

    return 0;
}

int jocky_polymorphic_mutate(uint8_t* code, int code_size, uint8_t* output, int* output_size) {
    if (!code || code_size <= 0 || !output || !output_size || *output_size < code_size + 32) {
        return -1;
    }

    memcpy(output, code, code_size);

    uint32_t mutations = 5 + (rand() % 10);
    int offset = 0;

    for (uint32_t i = 0; i < mutations; i++) {
        int mutation_type = rand() % 3;
        int mutation_offset = rand() % (code_size / 4);

        if (mutation_type == 0) {
            output[mutation_offset] ^= (rand() & 0xFF);
        } else if (mutation_type == 1) {
            output[mutation_offset] = (output[mutation_offset] + 1) & 0xFF;
        } else {
            output[mutation_offset] = (output[mutation_offset] - 1) & 0xFF;
        }
    }

    *output_size = code_size;
    return 0;
}

int jocky_inject_junk_code(uint8_t* code, int code_size, uint8_t* output, int* output_size) {
    if (!code || code_size <= 0 || !output || !output_size) {
        return -1;
    }

    int junk_size = (rand() % 256) + 64;
    int total_size = code_size + junk_size;

    if (*output_size < total_size) {
        return -1;
    }

    int insertion_point = rand() % (code_size / 2);

    memcpy(output, code, insertion_point);

    for (int i = 0; i < junk_size; i++) {
        output[insertion_point + i] = rand() & 0xFF;
    }

    memcpy(output + insertion_point + junk_size, code + insertion_point, code_size - insertion_point);

    *output_size = total_size;
    return 0;
}

int jocky_flatten_control_flow(uint8_t* code, int code_size, uint8_t* output, int* output_size) {
    if (!code || code_size <= 0 || !output || !output_size || *output_size < code_size) {
        return -1;
    }

    memcpy(output, code, code_size);

    int block_size = 16;
    for (int i = 0; i < code_size; i += block_size) {
        int current_block_size = (i + block_size < code_size) ? block_size : (code_size - i);

        for (int j = 0; j < current_block_size / 2; j++) {
            uint8_t temp = output[i + j];
            output[i + j] = output[i + current_block_size - 1 - j];
            output[i + current_block_size - 1 - j] = temp;
        }
    }

    *output_size = code_size;
    return 0;
}

int jocky_apply_string_obfuscation(uint8_t* data, int data_size, uint32_t key) {
    if (!data || data_size <= 0) {
        return -1;
    }

    for (int i = 0; i < data_size; i++) {
        if (data[i] == 0) {
            break;
        }
        data[i] ^= (key >> (8 * (i % 4))) & 0xFF;
    }

    return 0;
}

int jocky_mangle_function_name(const char* original_name, char* mangled_name, int max_len) {
    if (!original_name || !mangled_name || max_len <= 0) {
        return -1;
    }

    uint32_t hash = 5381;
    for (const char* p = original_name; *p; p++) {
        hash = ((hash << 5) + hash) ^ *p;
    }

    int name_len = snprintf(mangled_name, max_len, "_Z%u%s", hash, original_name);
    if (name_len < 0 || name_len >= max_len) {
        return -1;
    }

    return 0;
}
