#ifndef JOCKY_VM_H
#define JOCKY_VM_H

#include <stdint.h>
#include <stddef.h>
#include "vm_opcodes.h"

#ifdef __cplusplus
extern "C" {
#endif

void jocky_vm_init(void);
void jocky_vm_destroy(void);
void jocky_vm_load(uint8_t *bytecode, size_t size);
void jocky_vm_register_external(uint32_t index, void *func);
uint64_t jocky_vm_execute(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);
void *jocky_vm_compile_function(void *func_ptr, size_t *out_size);
uint64_t jocky_vm_get_register(uint32_t reg_idx);
void jocky_vm_set_register(uint32_t reg_idx, uint64_t value);

#ifdef __cplusplus
}
#endif

#endif
