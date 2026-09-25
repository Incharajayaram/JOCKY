// Linux module loading via dlopen/dlsym
// Cross-platform dynamic library loading for JOCKY

#include "../include/jocky_rt.h"
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__

// Handle opaque type for loaded modules
typedef struct {
    void* handle;
    char path[512];
} jocky_module_t;

/**
 * Load a shared library (.so file) at runtime.
 *
 * @param path Path to the .so file (absolute or relative)
 * @return Opaque module handle (cast to void*), or NULL on failure
 *
 * Example:
 *   void* mod = jocky_module_load("/lib/x86_64-linux-gnu/libc.so.6");
 *   if (!mod) return -1;  // Load failed
 */
void* jocky_module_load(const char* path) {
    if (!path) return NULL;

    // Load with RTLD_LAZY (lazy symbol binding) and RTLD_LOCAL (keep symbols private)
    void* handle = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
    if (!handle) {
        // Could log dlerror() here for debugging, but we keep it silent for operational OPSEC
        return NULL;
    }

    return handle;
}

/**
 * Unload a previously loaded module.
 *
 * @param handle Module handle from jocky_module_load
 * @return true on success, false on failure
 */
bool jocky_module_unload(void* handle) {
    if (!handle) return false;

    int result = dlclose(handle);
    return (result == 0);  // dlclose returns 0 on success
}

/**
 * Resolve a symbol (function/variable) from a loaded module.
 *
 * @param handle Module handle from jocky_module_load
 * @param symbol_name Name of the symbol to resolve
 * @return Pointer to the symbol, or NULL if not found
 *
 * Example:
 *   typedef int (*printf_fn)(const char*, ...);
 *   void* mod = jocky_module_load("libc.so.6");
 *   printf_fn printf_ptr = (printf_fn)jocky_module_symbol(mod, "printf");
 */
void* jocky_module_symbol(void* handle, const char* symbol_name) {
    if (!handle || !symbol_name) return NULL;

    // dlsym can legitimately return NULL for symbols with NULL addresses
    // So we clear dlerror() first, then check if dlsym returned NULL with an error
    dlerror();  // Clear any previous error
    void* symbol = dlsym(handle, symbol_name);

    // If dlsym returned NULL, check if there was an error
    if (!symbol && dlerror() != NULL) {
        return NULL;  // Symbol not found
    }

    return symbol;
}

/**
 * Load a module and get a symbol in one call.
 *
 * @param path Path to the .so file
 * @param symbol_name Name of the symbol to resolve
 * @return Pointer to the symbol, or NULL on any failure
 *
 * Example:
 *   void* printf_ptr = jocky_module_get_symbol("/lib/x86_64-linux-gnu/libc.so.6", "printf");
 */
void* jocky_module_get_symbol(const char* path, const char* symbol_name) {
    void* handle = jocky_module_load(path);
    if (!handle) return NULL;

    void* symbol = jocky_module_symbol(handle, symbol_name);

    // Don't unload immediately - caller may need it. They should call jocky_module_unload
    // when done. For immediate one-shot access, they can unload after calling the symbol.

    return symbol;
}

/**
 * Check if a symbol exists in a loaded module without resolving it.
 * Useful for feature detection.
 *
 * @param handle Module handle
 * @param symbol_name Name of the symbol to check
 * @return true if symbol exists, false otherwise
 */
bool jocky_module_has_symbol(void* handle, const char* symbol_name) {
    if (!handle || !symbol_name) return false;

    dlerror();  // Clear previous error
    void* symbol = dlsym(handle, symbol_name);

    return (symbol != NULL || dlerror() == NULL);
}

/**
 * Get the base address of a loaded module.
 * Parses /proc/self/maps to find the module's memory mapping.
 *
 * @param path Path to the .so file (or just filename)
 * @return Base address of the module, or 0 if not found or not loaded
 *
 * Note: This is useful for calculating offsets from a module base.
 */
uintptr_t jocky_module_base(const char* path) {
    if (!path) return 0;

    // Extract just the filename if a full path was provided
    const char* filename = path;
    for (const char* p = path; *p; p++) {
        if (*p == '/') filename = p + 1;
    }

    // Open /proc/self/maps and search for the module
    FILE* maps = fopen("/proc/self/maps", "r");
    if (!maps) return 0;

    char line[512];
    uintptr_t base = 0;

    while (fgets(line, sizeof(line), maps)) {
        // Lines look like: 7f1234567000-7f1234568000 r-xp 00000000 08:01 1234567    /path/to/lib.so
        // We search for the filename at the end

        if (strstr(line, filename)) {
            // Extract the base address (first hex value before '-')
            sscanf(line, "%lx-", &base);
            break;
        }
    }

    fclose(maps);
    return base;
}

#else
// Stub implementations for non-Linux platforms

void* jocky_module_load(const char* path) {
    (void)path;
    return NULL;
}

bool jocky_module_unload(void* handle) {
    (void)handle;
    return false;
}

void* jocky_module_symbol(void* handle, const char* symbol_name) {
    (void)handle;
    (void)symbol_name;
    return NULL;
}

void* jocky_module_get_symbol(const char* path, const char* symbol_name) {
    (void)path;
    (void)symbol_name;
    return NULL;
}

bool jocky_module_has_symbol(void* handle, const char* symbol_name) {
    (void)handle;
    (void)symbol_name;
    return false;
}

uintptr_t jocky_module_base(const char* path) {
    (void)path;
    return 0;
}

#endif  // __linux__
