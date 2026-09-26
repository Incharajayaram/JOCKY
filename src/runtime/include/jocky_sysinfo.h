#ifndef JOCKY_SYSINFO_H
#define JOCKY_SYSINFO_H

#include <stddef.h>
#include <stdint.h>

/* CPU Information */
typedef struct {
    int cores;                  /* Number of CPU cores */
    int logical_processors;     /* Number of logical processors */
    char brand[256];            /* CPU brand string */
    uint32_t family;            /* CPUID family */
    uint32_t model;             /* CPUID model */
    uint32_t stepping;          /* CPUID stepping */
    uint64_t features;          /* Feature flags (bitmask) */
    int has_avx;                /* AVX support */
    int has_sse42;              /* SSE4.2 support */
    int has_aes_ni;             /* AES-NI support */
} jocky_cpu_info_t;

/* Memory Information */
typedef struct {
    uint64_t total;             /* Total memory in bytes */
    uint64_t available;         /* Available memory in bytes */
    uint64_t free;              /* Free memory in bytes */
    uint64_t used;              /* Used memory in bytes */
    int percent_used;           /* Percentage used (0-100) */
} jocky_memory_info_t;

/* OS Information */
typedef struct {
    char os_name[128];          /* OS name (Linux, Android, etc.) */
    char kernel_release[128];   /* Kernel release (e.g., 5.10.0-8) */
    char kernel_version[256];   /* Full version string */
    char machine[64];           /* Machine type (x86_64, armv7l, etc.) */
    int bits;                   /* 32 or 64 */
} jocky_osinfo_t;

/* ========== CPU INFORMATION ========== */

/**
 * Get CPU information.
 * Returns 0 on success, -1 on failure.
 */
int jocky_sysinfo_cpu(jocky_cpu_info_t* out);

/**
 * Get number of CPU cores.
 * Returns number of cores, 0 on error.
 */
int jocky_sysinfo_cpu_cores(void);

/**
 * Check if CPU supports AVX.
 * Returns 1 if supported, 0 otherwise.
 */
int jocky_sysinfo_has_avx(void);

/**
 * Check if CPU supports AES-NI.
 * Returns 1 if supported, 0 otherwise.
 */
int jocky_sysinfo_has_aes_ni(void);

/* ========== MEMORY INFORMATION ========== */

/**
 * Get memory information.
 * Returns 0 on success, -1 on failure.
 */
int jocky_sysinfo_memory(jocky_memory_info_t* out);

/**
 * Get total system memory in bytes.
 * Returns memory size, 0 on error.
 */
uint64_t jocky_sysinfo_memory_total(void);

/**
 * Get available system memory in bytes.
 * Returns memory size, 0 on error.
 */
uint64_t jocky_sysinfo_memory_available(void);

/* ========== SYSTEM INFORMATION ========== */

/**
 * Get OS information.
 * Returns 0 on success, -1 on failure.
 */
int jocky_sysinfo_osinfo(jocky_osinfo_t* out);

/**
 * Get system uptime in seconds.
 * Returns uptime, 0 on error.
 */
uint64_t jocky_sysinfo_uptime(void);

/**
 * Get system hostname.
 * buffer must be at least 256 bytes.
 * Returns 0 on success, -1 on failure.
 */
int jocky_sysinfo_hostname(char* buffer, size_t size);

/**
 * Get kernel release string.
 * buffer must be at least 128 bytes.
 * Returns 0 on success, -1 on failure.
 */
int jocky_sysinfo_kernel_release(char* buffer, size_t size);

/**
 * Get machine architecture.
 * buffer must be at least 64 bytes.
 * Returns 0 on success, -1 on failure.
 */
int jocky_sysinfo_arch(char* buffer, size_t size);

/**
 * Get page size in bytes.
 * Returns page size, 0 on error.
 */
int jocky_sysinfo_pagesize(void);

/* ========== UTILITY ========== */

/**
 * Initialize sysinfo (one-time cache setup).
 * Returns 0 on success, -1 on failure.
 */
int jocky_sysinfo_init(void);

/**
 * Get system load average.
 * out should point to array of 3 doubles: 1, 5, 15 minute averages.
 * Returns 0 on success, -1 on failure.
 */
int jocky_sysinfo_load_average(double* out);

#endif // JOCKY_SYSINFO_H
