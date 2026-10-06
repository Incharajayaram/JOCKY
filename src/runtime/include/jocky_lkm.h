#ifndef JOCKY_LKM_H
#define JOCKY_LKM_H

#include <stdint.h>

int32_t jocky_lkm_load(const char* path, const char* module_name);
int32_t jocky_lkm_unload(const char* module_name);
int8_t* jocky_lkm_get_symbol(const char* module_name, const char* symbol);
int32_t jocky_lkm_hook_syscall(int32_t syscall_num, void* hook_fn);
int32_t jocky_lkm_unhook_syscall(int32_t syscall_num);
int32_t jocky_lkm_get_syscall_table(uint64_t* out_addr);

#endif
