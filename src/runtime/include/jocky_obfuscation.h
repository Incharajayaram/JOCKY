#ifndef JOCKY_OBFUSCATION_H
#define JOCKY_OBFUSCATION_H

#include <stdint.h>
#include <stddef.h>

typedef void* jocky_code_patch_t;
typedef void (*jocky_code_callback_t)(void* context);

int jocky_enable_code_mutation(void);

int jocky_disable_code_mutation(void);

int jocky_patch_code(void* target_addr, const uint8_t* patch_code, int patch_size);

int jocky_restore_code(void* target_addr, int patch_size);

int jocky_generate_runtime_stub(const char* function_name, uint8_t* output, int* output_size);

int jocky_hook_function(void* original_func, void* hook_func, jocky_code_patch_t* patch_handle);

int jocky_unhook_function(jocky_code_patch_t patch_handle);

int jocky_allocate_code_buffer(int size, void** buffer_addr);

int jocky_free_code_buffer(void* buffer_addr, int size);

int jocky_make_code_executable(void* addr, int size);

int jocky_make_code_writable(void* addr, int size);

int jocky_mutate_based_on_analysis(void);

int jocky_adaptive_obfuscation_step(void);

#endif
