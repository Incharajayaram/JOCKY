/**
 * Loadable Kernel Module (LKM) Loader
 *
 * Handles loading and unloading of Linux kernel modules dynamically.
 * Requires CONFIG_MODULES kernel configuration.
 */

#pragma once

#include <sys/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LKM metadata */
typedef struct {
    const char* name;
    const char* path;
    char* params;
    uint64_t load_addr;
    size_t size;
} jocky_lkm_t;

/* Load kernel module from file */
int jocky_lkm_load(const char* path, jocky_lkm_t* out_module);

/* Unload kernel module by name */
int jocky_lkm_unload(const char* name);

/* Get module base address */
uint64_t jocky_lkm_base(const char* name);

/* List all loaded modules */
int jocky_lkm_list(jocky_lkm_t** modules, size_t* count);

/* Free module list */
void jocky_lkm_list_free(jocky_lkm_t* modules);

#ifdef __cplusplus
}
#endif
