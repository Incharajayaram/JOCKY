#include "../include/jocky_util.h"
#include "../include/jocky_syscall.h"
#include <string.h>
#include <stdio.h>

/* CLOCK_REALTIME = 0, CLOCK_MONOTONIC = 1 */

/* ========== TIME UTILITIES ========== */

long jocky_time(void) {
    return jocky_syscall1(SYS_time, 0);
}

int jocky_sleep_ms(uint64_t milliseconds) {
    if (milliseconds == 0) return 0;

    /* Convert milliseconds to seconds + nanoseconds */
    struct {
        long tv_sec;
        long tv_nsec;
    } ts;

    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;

    struct {
        long tv_sec;
        long tv_nsec;
    } rem;

    long result = jocky_syscall2(SYS_nanosleep, (long)&ts, (long)&rem);
    return (result == 0) ? 0 : -1;
}

long jocky_clock_gettime(int clock_id, uint64_t* ns) {
    if (!ns) return -1;

    struct {
        long tv_sec;
        long tv_nsec;
    } ts;

    long result = jocky_syscall2(SYS_clock_gettime, clock_id, (long)&ts);
    if (result != 0) return -1;

    *ns = (uint64_t)ts.tv_sec * 1000000000UL + (uint64_t)ts.tv_nsec;
    return 0;
}

/* ========== STRING UTILITIES ========== */

int jocky_strncpy_safe(char* dst, const char* src, size_t dstsize) {
    if (!dst || !src || dstsize == 0) return -1;

    size_t srclen = strlen(src);
    if (srclen >= dstsize) return -1;  /* Buffer too small */

    strncpy(dst, src, dstsize - 1);
    dst[dstsize - 1] = '\0';

    return 0;
}

int jocky_strncat_safe(char* dst, const char* src, size_t dstsize) {
    if (!dst || !src || dstsize == 0) return -1;

    size_t dstlen = strlen(dst);
    size_t srclen = strlen(src);

    if (dstlen + srclen >= dstsize) return -1;  /* Buffer too small */

    strncat(dst, src, dstsize - dstlen - 1);

    return 0;
}

size_t jocky_strlen(const char* str) {
    if (!str) return 0;
    return strlen(str);
}

int jocky_strcmp(const char* s1, const char* s2) {
    if (!s1 || !s2) return -1;
    return strcmp(s1, s2);
}

int jocky_strncmp(const char* s1, const char* s2, size_t n) {
    if (!s1 || !s2) return -1;
    return strncmp(s1, s2, n);
}

char* jocky_strchr(const char* str, char c) {
    if (!str) return NULL;
    return strchr(str, c);
}

char* jocky_strrchr(const char* str, char c) {
    if (!str) return NULL;
    return strrchr(str, c);
}

/* ========== ERROR HANDLING ========== */

/* Simple errno value (not thread-safe) */
static int jocky_errno = 0;

const char* jocky_strerror(int errnum) {
    static const char* errors[] = {
        "Success",                          /* 0 */
        "Operation not permitted",          /* 1 */
        "No such file or directory",        /* 2 */
        "No such process",                  /* 3 */
        "Interrupted system call",          /* 4 */
        "Input/output error",               /* 5 */
        "No such device or address",        /* 6 */
        "Argument list too long",           /* 7 */
        "Exec format error",                /* 8 */
        "Bad file number",                  /* 9 */
        "No child processes",               /* 10 */
    };

    if (errnum < 0 || errnum > 10) {
        return "Unknown error";
    }

    return errors[errnum];
}

int jocky_get_errno(void) {
    return jocky_errno;
}

void jocky_set_errno(int errnum) {
    jocky_errno = errnum;
}

/* ========== GENERIC SYSCALL ========== */

long jocky_syscall_generic(long number, long arg1, long arg2, long arg3,
                           long arg4, long arg5, long arg6) {
    return jocky_syscall6(number, arg1, arg2, arg3, arg4, arg5, arg6);
}
