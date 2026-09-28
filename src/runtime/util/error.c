/**
 * JOCKY Error Handling & Memory Tracking Implementation
 *
 * Provides comprehensive error handling, memory leak detection, and safe
 * memory operations with null pointer validation.
 */

#include "jocky_error.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define JOCKY_MAX_TRACKED_BLOCKS 50000

typedef struct {
    jocky_mem_block_t *blocks;
    uint32_t count;
    uint32_t capacity;
    size_t total_allocated;
    bool enabled;
} jocky_mem_tracker_t;

static jocky_mem_tracker_t g_mem_tracker = {0};
static bool g_error_logging_enabled = false;

void jocky_mem_track_init(void) {
    if (g_mem_tracker.blocks) return;

    g_mem_tracker.capacity = JOCKY_MAX_TRACKED_BLOCKS;
    g_mem_tracker.blocks = malloc(sizeof(jocky_mem_block_t) * g_mem_tracker.capacity);
    if (!g_mem_tracker.blocks) {
        fprintf(stderr, "JOCKY: Memory tracking initialization failed\n");
        return;
    }

    memset(g_mem_tracker.blocks, 0, sizeof(jocky_mem_block_t) * g_mem_tracker.capacity);
    g_mem_tracker.count = 0;
    g_mem_tracker.total_allocated = 0;
    g_mem_tracker.enabled = true;
}

void* jocky_alloc_tracked(size_t size, const char *file, int line) {
    if (size == 0) {
        if (g_error_logging_enabled) {
            JOCKY_ERROR_LOG(JOCKY_ERR_INVALID_SIZE, "Attempted to allocate zero bytes");
        }
        return NULL;
    }

    if (!g_mem_tracker.enabled) {
        jocky_mem_track_init();
    }

    if (g_mem_tracker.count >= g_mem_tracker.capacity) {
        if (g_error_logging_enabled) {
            fprintf(stderr, "JOCKY: Tracking table full, allocation limit reached\n");
        }
        return NULL;
    }

    void *ptr = malloc(size);
    if (!ptr) {
        if (g_error_logging_enabled) {
            JOCKY_ERROR_LOG(JOCKY_ERR_ALLOCATION_FAILED, "malloc returned NULL");
        }
        return NULL;
    }

    memset(ptr, 0, size);

    jocky_mem_block_t *block = &g_mem_tracker.blocks[g_mem_tracker.count];
    block->ptr = ptr;
    block->size = size;
    block->source_file = file;
    block->source_line = line;
    block->alloc_id = g_mem_tracker.count;

    g_mem_tracker.count++;
    g_mem_tracker.total_allocated += size;

    return ptr;
}

int32_t jocky_free_tracked(void *ptr, const char *file, int line) {
    if (!ptr) return JOCKY_SUCCESS;

    if (!g_mem_tracker.enabled) return JOCKY_ERR_INVALID_PARAM;

    for (uint32_t i = 0; i < g_mem_tracker.count; i++) {
        if (g_mem_tracker.blocks[i].ptr == ptr) {
            g_mem_tracker.total_allocated -= g_mem_tracker.blocks[i].size;

            memmove(&g_mem_tracker.blocks[i],
                    &g_mem_tracker.blocks[i + 1],
                    (g_mem_tracker.count - i - 1) * sizeof(jocky_mem_block_t));

            g_mem_tracker.count--;
            free(ptr);
            return JOCKY_SUCCESS;
        }
    }

    if (g_error_logging_enabled) {
        fprintf(stderr, "JOCKY: Double-free or untracked pointer at %s:%d\n", file, line);
    }
    return JOCKY_ERR_DOUBLE_FREE;
}

void jocky_mem_dump_leaks(void) {
    if (!g_mem_tracker.enabled) return;

    if (g_mem_tracker.count == 0) {
        printf("JOCKY: No memory leaks detected\n");
        return;
    }

    printf("JOCKY: Memory Leak Report\n");
    printf("=========================\n");
    printf("Total Leaks: %u allocations, %zu bytes\n\n", g_mem_tracker.count,
           g_mem_tracker.total_allocated);

    for (uint32_t i = 0; i < g_mem_tracker.count; i++) {
        jocky_mem_block_t *block = &g_mem_tracker.blocks[i];
        if (block->ptr) {
            printf("  [%u] %zu bytes at %s:%d (ptr: %p)\n",
                   block->alloc_id, block->size, block->source_file,
                   block->source_line, block->ptr);
        }
    }
}

size_t jocky_mem_total_allocated(void) {
    return g_mem_tracker.total_allocated;
}

uint32_t jocky_mem_allocation_count(void) {
    return g_mem_tracker.count;
}

void jocky_mem_track_clear(void) {
    if (!g_mem_tracker.blocks) return;

    for (uint32_t i = 0; i < g_mem_tracker.count; i++) {
        if (g_mem_tracker.blocks[i].ptr) {
            free(g_mem_tracker.blocks[i].ptr);
        }
    }

    memset(g_mem_tracker.blocks, 0, sizeof(jocky_mem_block_t) * g_mem_tracker.capacity);
    g_mem_tracker.count = 0;
    g_mem_tracker.total_allocated = 0;
}

void jocky_safe_free(void **ptr) {
    if (!ptr || !*ptr) return;

    free(*ptr);
    *ptr = NULL;
}

void* jocky_safe_memcpy(void *dst, const void *src, size_t len) {
    if (!dst || !src) return NULL;
    if (len == 0) return dst;

    return memcpy(dst, src, len);
}

void* jocky_safe_memset(void *ptr, int value, size_t len) {
    if (!ptr) return NULL;
    if (len == 0) return ptr;

    return memset(ptr, value, len);
}

void* jocky_safe_memmove(void *dst, const void *src, size_t len) {
    if (!dst || !src) return NULL;
    if (len == 0) return dst;

    return memmove(dst, src, len);
}

int32_t jocky_safe_strcpy(char *dst, size_t dst_size, const char *src) {
    if (!dst || !src || dst_size == 0) return JOCKY_ERR_NULL_PTR;

    size_t src_len = strlen(src);
    if (src_len >= dst_size) return JOCKY_ERR_BUFFER_OVERFLOW;

    memcpy(dst, src, src_len + 1);
    return JOCKY_SUCCESS;
}

int32_t jocky_safe_strcat(char *dst, size_t dst_size, const char *src) {
    if (!dst || !src || dst_size == 0) return JOCKY_ERR_NULL_PTR;

    size_t dst_len = strlen(dst);
    size_t src_len = strlen(src);

    if (dst_len + src_len >= dst_size) return JOCKY_ERR_BUFFER_OVERFLOW;

    memcpy(dst + dst_len, src, src_len + 1);
    return JOCKY_SUCCESS;
}

size_t jocky_safe_strlen(const char *str) {
    return str ? strlen(str) : 0;
}

int32_t jocky_safe_strcmp(const char *s1, const char *s2) {
    if (!s1 || !s2) return JOCKY_ERR_NULL_PTR;
    return strcmp(s1, s2);
}

const char* jocky_error_message(jocky_error_t err) {
    switch (err) {
        case JOCKY_SUCCESS: return "Success";
        case JOCKY_ERR_NULL_PTR: return "Null pointer";
        case JOCKY_ERR_INVALID_SIZE: return "Invalid size";
        case JOCKY_ERR_ALLOCATION_FAILED: return "Memory allocation failed";
        case JOCKY_ERR_OPERATION_FAILED: return "Operation failed";
        case JOCKY_ERR_INVALID_PARAM: return "Invalid parameter";
        case JOCKY_ERR_BUFFER_OVERFLOW: return "Buffer overflow";
        case JOCKY_ERR_DIVISION_BY_ZERO: return "Division by zero";
        case JOCKY_ERR_INTEGER_OVERFLOW: return "Integer overflow";
        case JOCKY_ERR_USE_AFTER_FREE: return "Use after free";
        case JOCKY_ERR_DOUBLE_FREE: return "Double free";
        default: return "Unknown error";
    }
}

void jocky_error_logging_enable(bool enable) {
    g_error_logging_enabled = enable;
}

void jocky_error_log(const char *func, const char *file, int line,
                     jocky_error_t err, const char *msg) {
    if (!g_error_logging_enabled) return;

    fprintf(stderr, "JOCKY ERROR: %s (%d)\n", jocky_error_message(err), err);
    fprintf(stderr, "  Function: %s\n", func);
    fprintf(stderr, "  Location: %s:%d\n", file, line);
    if (msg) {
        fprintf(stderr, "  Message:  %s\n", msg);
    }
}
