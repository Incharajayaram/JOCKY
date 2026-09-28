#ifndef JOCKY_LKM_H
#define JOCKY_LKM_H

#include <stdint.h>

/* Linux Kernel Module (LKM) loading and hooking */

typedef struct {
    char name[256];
    char path[512];
    uint64_t base_address;
    uint32_t size;
    int loaded;
} JOCKY_LKM;

/* Load kernel module */
int jocky_lkm_load(const char* path, JOCKY_LKM* out_module);

/* Unload kernel module */
int jocky_lkm_unload(JOCKY_LKM* module);

/* Hook system call */
int jocky_lkm_hook_syscall(
    JOCKY_LKM* module,
    int syscall_num,
    void* hook_function);

/* Unhook system call */
int jocky_lkm_unhook_syscall(int syscall_num);

/* Get system call table address */
int jocky_lkm_get_syscall_table(uint64_t* out_table_addr);

/* Execute kernel function from module */
int jocky_lkm_execute(JOCKY_LKM* module, const char* function_name);

/* Query module status */
int jocky_lkm_query(JOCKY_LKM* module);

#endif
