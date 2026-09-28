#ifndef JOCKY_OBFUSCATION_H
#define JOCKY_OBFUSCATION_H

#include <stdint.h>
#include <stddef.h>

typedef void* jocky_obfuscation_ctx_t;

#define JOCKY_XOR_OBFUSCATE 1
#define JOCKY_ROT_OBFUSCATE 2
#define JOCKY_SHUFFLE_OBFUSCATE 3

int jocky_obfuscate_code(uint8_t* code, int code_size, int method, uint32_t key);

int jocky_deobfuscate_code(uint8_t* code, int code_size, int method, uint32_t key);

uint32_t jocky_generate_obfuscation_key(void);

int jocky_polymorphic_mutate(uint8_t* code, int code_size, uint8_t* output, int* output_size);

int jocky_inject_junk_code(uint8_t* code, int code_size, uint8_t* output, int* output_size);

int jocky_flatten_control_flow(uint8_t* code, int code_size, uint8_t* output, int* output_size);

int jocky_apply_string_obfuscation(uint8_t* data, int data_size, uint32_t key);

int jocky_mangle_function_name(const char* original_name, char* mangled_name, int max_len);

#endif
