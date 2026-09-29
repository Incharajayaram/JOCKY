#ifndef JOCKY_ERROR_H
#define JOCKY_ERROR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Error Codes
 * ============================================================================ */

typedef enum {
    JOCKY_SUCCESS = 0,
    JOCKY_ERR_NULL_PTR = -1,
    JOCKY_ERR_INVALID_SIZE = -2,
    JOCKY_ERR_ALLOCATION_FAILED = -3,
    JOCKY_ERR_OPERATION_FAILED = -4,
    JOCKY_ERR_INVALID_PARAM = -5,
    JOCKY_ERR_BUFFER_OVERFLOW = -6,
    JOCKY_ERR_DIVISION_BY_ZERO = -7,
    JOCKY_ERR_INTEGER_OVERFLOW = -8,
    JOCKY_ERR_USE_AFTER_FREE = -9,
    JOCKY_ERR_DOUBLE_FREE = -10,
} jocky_error_t;

/* ============================================================================
 * Memory Tracking System
 * ============================================================================ */

typedef struct {
    void *ptr;
    size_t size;
    const char *source_file;
    int source_line;
    uint32_t alloc_id;
} jocky_mem_block_t;

/* Initialize memory tracking system */
void jocky_mem_track_init(void);

/* Allocate and track memory */
void* jocky_alloc_tracked(size_t size, const char *file, int line);

/* Free tracked memory */
int32_t jocky_free_tracked(void *ptr, const char *file, int line);

/* Dump memory leak report */
void jocky_mem_dump_leaks(void);

/* Get total allocated bytes */
size_t jocky_mem_total_allocated(void);

/* Get number of tracked allocations */
uint32_t jocky_mem_allocation_count(void);

/* Clear all tracking (for testing) */
void jocky_mem_track_clear(void);

/* Macro helpers for tracking */
#define JOCKY_ALLOC_TRACKED(size) \
    jocky_alloc_tracked(size, __FILE__, __LINE__)

#define JOCKY_FREE_TRACKED(ptr) \
    jocky_free_tracked(ptr, __FILE__, __LINE__)

/* ============================================================================
 * Safe Memory Operations
 * ============================================================================ */

/* Safe free that prevents use-after-free */
void jocky_safe_free(void **ptr);

/* Safe memcpy with validation */
void* jocky_safe_memcpy(void *dst, const void *src, size_t len);

/* Safe memset with validation */
void* jocky_safe_memset(void *ptr, int value, size_t len);

/* Safe memmove with validation */
void* jocky_safe_memmove(void *dst, const void *src, size_t len);

/* ============================================================================
 * String Safety Functions
 * ============================================================================ */

/* Safe string copy with null termination */
int32_t jocky_safe_strcpy(char *dst, size_t dst_size, const char *src);

/* Safe string concatenation */
int32_t jocky_safe_strcat(char *dst, size_t dst_size, const char *src);

/* Get string length safely (returns 0 for NULL) */
size_t jocky_safe_strlen(const char *str);

/* Safe string comparison */
int32_t jocky_safe_strcmp(const char *s1, const char *s2);

/* ============================================================================
 * Error Reporting and Debugging
 * ============================================================================ */

/* Get human-readable error message */
const char* jocky_error_message(jocky_error_t err);

/* Enable/disable error logging */
void jocky_error_logging_enable(bool enable);

/* Log an error with context */
void jocky_error_log(const char *func, const char *file, int line,
                     jocky_error_t err, const char *msg);

/* Macro for error logging */
#define JOCKY_ERROR_LOG(err, msg) \
    jocky_error_log(__func__, __FILE__, __LINE__, err, msg)

#ifdef __cplusplus
}
#endif

#endif
