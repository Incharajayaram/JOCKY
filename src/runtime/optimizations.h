/**
 * Runtime Performance Optimizations for JOCKY
 *
 * Provides fast paths for common operations and caching mechanisms
 * to reduce runtime overhead.
 */

#ifndef JOCKY_RUNTIME_OPTIMIZATIONS_H
#define JOCKY_RUNTIME_OPTIMIZATIONS_H

#include <stdint.h>
#include <string.h>
#include <stddef.h>

/* Fast memory copy for small aligned buffers */
static inline void jocky_fast_memcpy_i32(uint32_t *dst, const uint32_t *src, size_t count) {
    while (count-- > 0) {
        *dst++ = *src++;
    }
}

static inline void jocky_fast_memcpy_i64(uint64_t *dst, const uint64_t *src, size_t count) {
    while (count-- > 0) {
        *dst++ = *src++;
    }
}

/* Fast memory fill for optimization */
static inline void jocky_fast_memset_i32(uint32_t *dst, uint32_t value, size_t count) {
    while (count-- > 0) {
        *dst++ = value;
    }
}

/* Simple module cache for frequently accessed modules */
#define JOCKY_MODULE_CACHE_SIZE 16

typedef struct {
    const char *name;
    void *handle;
} ModuleCacheEntry;

static ModuleCacheEntry cached_modules[JOCKY_MODULE_CACHE_SIZE];
static int cached_module_count = 0;

/* Get cached module or load if not cached */
static inline void* jocky_get_module_cached(const char *name, void* (*loader)(const char*)) {
    for (int i = 0; i < cached_module_count; i++) {
        if (cached_modules[i].name && strcmp(cached_modules[i].name, name) == 0) {
            return cached_modules[i].handle;
        }
    }

    if (cached_module_count < JOCKY_MODULE_CACHE_SIZE) {
        void *handle = loader(name);
        if (handle) {
            cached_modules[cached_module_count].name = name;
            cached_modules[cached_module_count].handle = handle;
            cached_module_count++;
        }
        return handle;
    }

    return loader(name);
}

/* Object pool for frequently allocated small strings */
typedef struct {
    char buffer[256];
    int in_use;
} StringBuffer;

#define JOCKY_STRING_POOL_SIZE 64

static StringBuffer string_pool[JOCKY_STRING_POOL_SIZE];
static int string_pool_init = 0;

static inline void jocky_string_pool_init(void) {
    if (!string_pool_init) {
        memset(string_pool, 0, sizeof(string_pool));
        string_pool_init = 1;
    }
}

static inline char* jocky_get_string_buffer(void) {
    jocky_string_pool_init();
    for (int i = 0; i < JOCKY_STRING_POOL_SIZE; i++) {
        if (!string_pool[i].in_use) {
            string_pool[i].in_use = 1;
            string_pool[i].buffer[0] = '\0';
            return string_pool[i].buffer;
        }
    }
    return NULL;
}

static inline void jocky_return_string_buffer(char *buf) {
    if (!buf) return;
    for (int i = 0; i < JOCKY_STRING_POOL_SIZE; i++) {
        if (string_pool[i].buffer == buf) {
            string_pool[i].in_use = 0;
            return;
        }
    }
}

/* Fast inline comparisons for common operations */
static inline int jocky_eq_i32(int32_t a, int32_t b) {
    return a == b;
}

static inline int jocky_eq_i64(int64_t a, int64_t b) {
    return a == b;
}

static inline int jocky_lt_i32(int32_t a, int32_t b) {
    return a < b;
}

static inline int jocky_lt_i64(int64_t a, int64_t b) {
    return a < b;
}

/* Prefetch optimization hints for large data structures */
#ifdef __GNUC__
#define JOCKY_PREFETCH(addr) __builtin_prefetch((addr), 0, 3)
#else
#define JOCKY_PREFETCH(addr)
#endif

/* Branch prediction hints */
#ifdef __GNUC__
#define JOCKY_LIKELY(x) __builtin_expect(!!(x), 1)
#define JOCKY_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define JOCKY_LIKELY(x) (x)
#define JOCKY_UNLIKELY(x) (x)
#endif

#endif /* JOCKY_RUNTIME_OPTIMIZATIONS_H */
