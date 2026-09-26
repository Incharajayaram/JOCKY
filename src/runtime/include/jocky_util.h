#ifndef JOCKY_UTIL_H
#define JOCKY_UTIL_H

#include <stddef.h>
#include <stdint.h>
#include <time.h>

/* ========== TIME UTILITIES ========== */

/**
 * Get current time in seconds since epoch.
 * Returns time_t value.
 */
long jocky_time(void);

/**
 * Sleep for specified milliseconds.
 * Returns 0 on success, -1 on error.
 */
int jocky_sleep_ms(uint64_t milliseconds);

/**
 * Get current time with nanosecond precision.
 * clock_id: CLOCK_REALTIME, CLOCK_MONOTONIC, etc.
 * Returns time in nanoseconds, -1 on error.
 */
long jocky_clock_gettime(int clock_id, uint64_t* ns);

/* ========== STRING UTILITIES ========== */

/**
 * Safe string copy.
 * Returns 0 on success, -1 on buffer overflow.
 */
int jocky_strncpy_safe(char* dst, const char* src, size_t dstsize);

/**
 * Safe string concatenation.
 * Returns 0 on success, -1 on buffer overflow.
 */
int jocky_strncat_safe(char* dst, const char* src, size_t dstsize);

/**
 * String length.
 * Returns length of string.
 */
size_t jocky_strlen(const char* str);

/**
 * String comparison.
 * Returns 0 if equal, <0 if s1 < s2, >0 if s1 > s2.
 */
int jocky_strcmp(const char* s1, const char* s2);

/**
 * String comparison (first n bytes).
 * Returns 0 if equal, <0 if s1 < s2, >0 if s1 > s2.
 */
int jocky_strncmp(const char* s1, const char* s2, size_t n);

/**
 * Find character in string.
 * Returns pointer to first occurrence, NULL if not found.
 */
char* jocky_strchr(const char* str, char c);

/**
 * Find last character in string.
 * Returns pointer to last occurrence, NULL if not found.
 */
char* jocky_strrchr(const char* str, char c);

/* ========== ERROR HANDLING ========== */

/**
 * Get error message from errno.
 * Returns pointer to error string.
 */
const char* jocky_strerror(int errnum);

/**
 * Get current errno value.
 * Returns errno (or 0 if no error).
 */
int jocky_get_errno(void);

/**
 * Set errno value.
 */
void jocky_set_errno(int errnum);

/* ========== GENERIC SYSCALL ========== */

/**
 * Generic syscall wrapper for advanced use.
 * Returns syscall result, -1 on error.
 */
long jocky_syscall_generic(long number, long arg1, long arg2, long arg3,
                           long arg4, long arg5, long arg6);

#endif // JOCKY_UTIL_H
