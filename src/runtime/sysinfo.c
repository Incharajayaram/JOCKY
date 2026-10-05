#define _GNU_SOURCE
#include "../include/jocky_sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <netdb.h>
#endif

/* CPUID function (x86/x64 only) */
#ifdef __GNUC__
static inline void cpuid(uint32_t eax, uint32_t ecx, uint32_t* regs) {
    #if defined(__x86_64__) || defined(__i386__)
    __asm__ __volatile__(
        "cpuid"
        : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
        : "0" (eax), "2" (ecx)
    );
    #else
    memset(regs, 0, sizeof(uint32_t) * 4);
    #endif
}
#endif

static int cpu_info_cached = 0;
static jocky_cpu_info_t cpu_cache;

int jocky_sysinfo_cpu(jocky_cpu_info_t* out) {
    if (!out) return -1;

    if (cpu_info_cached) {
        memcpy(out, &cpu_cache, sizeof(jocky_cpu_info_t));
        return 0;
    }

    memset(out, 0, sizeof(jocky_cpu_info_t));

#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    out->cores = (int)si.dwNumberOfProcessors;
    if (out->cores <= 0) out->cores = 1;
    out->logical_processors = out->cores;
#else
    /* Get core count */
    out->cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (out->cores <= 0) out->cores = 1;
    out->logical_processors = sysconf(_SC_NPROCESSORS_CONF);
    if (out->logical_processors <= 0) out->logical_processors = out->cores;

    /* Get CPU brand via /proc/cpuinfo */
    FILE* f = fopen("/proc/cpuinfo", "r");
    if (f) {
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            if (strncmp(line, "model name", 10) == 0) {
                char* colon = strchr(line, ':');
                if (colon) {
                    colon++;
                    while (*colon == ' ') colon++;
                    size_t len = strlen(colon);
                    if (len > 0 && colon[len-1] == '\n') len--;
                    if (len > sizeof(out->brand) - 1) len = sizeof(out->brand) - 1;
                    strncpy(out->brand, colon, len);
                    out->brand[len] = '\0';
                    break;
                }
            }
        }
        fclose(f);
    }
#endif

    /* Get CPU features via CPUID (x86/x64 only) */
    #ifdef __GNUC__
    #if defined(__x86_64__) || defined(__i386__)
    uint32_t regs[4];

    /* Get CPUID(1) for basic features */
    cpuid(1, 0, regs);
    out->family = (regs[0] >> 8) & 0xF;
    out->model = (regs[0] >> 4) & 0xF;
    out->stepping = regs[0] & 0xF;

    /* ECX contains feature bits */
    uint32_t ecx = regs[2];
    uint32_t edx = regs[3];

    out->has_aes_ni = (ecx >> 25) & 1;
    out->has_sse42 = (ecx >> 20) & 1;

    /* Check for AVX (bit 28 of ECX) */
    out->has_avx = (ecx >> 28) & 1;
    #endif
    #endif

    memcpy(&cpu_cache, out, sizeof(jocky_cpu_info_t));
    cpu_info_cached = 1;

    return 0;
}

int jocky_sysinfo_cpu_cores(void) {
    jocky_cpu_info_t info;
    if (jocky_sysinfo_cpu(&info) != 0) return 0;
    return info.cores;
}

int jocky_sysinfo_has_avx(void) {
    jocky_cpu_info_t info;
    if (jocky_sysinfo_cpu(&info) != 0) return 0;
    return info.has_avx;
}

int jocky_sysinfo_has_aes_ni(void) {
    jocky_cpu_info_t info;
    if (jocky_sysinfo_cpu(&info) != 0) return 0;
    return info.has_aes_ni;
}

int jocky_sysinfo_memory(jocky_memory_info_t* out) {
    if (!out) return -1;

#ifdef _WIN32
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof(ms);
    if (!GlobalMemoryStatusEx(&ms)) return -1;
    out->total = ms.ullTotalPhys;
    out->free = ms.ullAvailPhys;
    out->available = ms.ullAvailPhys;
    out->used = out->total - out->free;
    out->percent_used = (out->total > 0) ? (int)(100 * out->used / out->total) : 0;
#else
    struct sysinfo si;
    if (sysinfo(&si) != 0) return -1;
    out->total = si.totalram * si.mem_unit;
    out->free = si.freeram * si.mem_unit;
    out->available = out->free + (si.bufferram * si.mem_unit);
    out->used = out->total - out->free;
    out->percent_used = (out->total > 0) ? (int)(100 * out->used / out->total) : 0;
#endif

    return 0;
}

uint64_t jocky_sysinfo_memory_total(void) {
    jocky_memory_info_t info;
    if (jocky_sysinfo_memory(&info) != 0) return 0;
    return info.total;
}

uint64_t jocky_sysinfo_memory_available(void) {
    jocky_memory_info_t info;
    if (jocky_sysinfo_memory(&info) != 0) return 0;
    return info.available;
}

int jocky_sysinfo_osinfo(jocky_osinfo_t* out) {
    if (!out) return -1;

    memset(out, 0, sizeof(jocky_osinfo_t));

#ifdef _WIN32
    strncpy(out->os_name, "Windows", sizeof(out->os_name) - 1);
    OSVERSIONINFOEXA vi;
    memset(&vi, 0, sizeof(vi));
    vi.dwOSVersionInfoSize = sizeof(vi);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    GetVersionExA((OSVERSIONINFOA*)&vi);
#pragma GCC diagnostic pop
    snprintf(out->kernel_release, sizeof(out->kernel_release), "%lu.%lu",
             vi.dwMajorVersion, vi.dwMinorVersion);
    snprintf(out->kernel_version, sizeof(out->kernel_version), "Build %lu",
             vi.dwBuildNumber);
#if defined(_WIN64)
    strncpy(out->machine, "x86_64", sizeof(out->machine) - 1);
    out->bits = 64;
#else
    strncpy(out->machine, "i686", sizeof(out->machine) - 1);
    out->bits = 32;
#endif
#else
    struct utsname uts;
    if (uname(&uts) != 0) return -1;
    strncpy(out->os_name, uts.sysname, sizeof(out->os_name) - 1);
    strncpy(out->kernel_release, uts.release, sizeof(out->kernel_release) - 1);
    strncpy(out->kernel_version, uts.version, sizeof(out->kernel_version) - 1);
    strncpy(out->machine, uts.machine, sizeof(out->machine) - 1);
    if (strstr(out->machine, "64") != NULL || strcmp(out->machine, "aarch64") == 0)
        out->bits = 64;
    else
        out->bits = 32;
#endif

    return 0;
}

uint64_t jocky_sysinfo_uptime(void) {
#ifdef _WIN32
    return (uint64_t)(GetTickCount64() / 1000ULL);
#else
    struct sysinfo si;
    if (sysinfo(&si) != 0) return 0;
    return (uint64_t)si.uptime;
#endif
}

int jocky_sysinfo_hostname(char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

#ifdef _WIN32
    DWORD sz = (DWORD)size;
    if (!GetComputerNameA(buffer, &sz)) return -1;
#else
    if (gethostname(buffer, size) != 0) return -1;
#endif

    buffer[size - 1] = '\0';
    return 0;
}

int jocky_sysinfo_kernel_release(char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

    jocky_osinfo_t info;
    if (jocky_sysinfo_osinfo(&info) != 0) return -1;
    strncpy(buffer, info.kernel_release, size - 1);
    buffer[size - 1] = '\0';
    return 0;
}

int jocky_sysinfo_arch(char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

    jocky_osinfo_t info;
    if (jocky_sysinfo_osinfo(&info) != 0) return -1;
    strncpy(buffer, info.machine, size - 1);
    buffer[size - 1] = '\0';
    return 0;
}

int jocky_sysinfo_pagesize(void) {
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return (int)si.dwPageSize;
#else
    long pagesize = sysconf(_SC_PAGESIZE);
    return (pagesize > 0) ? (int)pagesize : 4096;
#endif
}

int jocky_sysinfo_init(void) {
    jocky_cpu_info_t dummy;
    return jocky_sysinfo_cpu(&dummy);
}

int jocky_sysinfo_load_average(double* out) {
    if (!out) return -1;

#ifdef _WIN32
    out[0] = out[1] = out[2] = -1.0;
    return -1;
#else
    if (getloadavg(out, 3) != 3) return -1;
    return 0;
#endif
}
