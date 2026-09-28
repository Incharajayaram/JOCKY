#include "../include/jocky_sysinfo.h"
#include "../include/jocky_syscall.h"
#include <string.h>
#include <stdio.h>

/* sysinfo structure from Linux kernel (x86_64) */
struct linux_sysinfo {
    long uptime;
    unsigned long loads[3];
    unsigned long totalram;
    unsigned long freeram;
    unsigned long sharedram;
    unsigned long bufferram;
    unsigned long totalswap;
    unsigned long freeswap;
    unsigned short procs;
    unsigned short pad;
    unsigned long totalhigh;
    unsigned long freehigh;
    unsigned int mem_unit;
    char _f[20 - 2*sizeof(long) - sizeof(int)];
};

/* utsname structure */
struct utsname {
    char sysname[65];
    char nodename[65];
    char release[65];
    char version[65];
    char machine[65];
    char domainname[65];
};

/* CPU info cache */
static int cpu_cached = 0;
static jocky_cpu_info_t cpu_cache;

/* Parse /proc/cpuinfo via read syscall */
static int read_proc_cpuinfo(char* brand, size_t brand_size) {
    long fd = jocky_syscall2(SYS_open, (long)"/proc/cpuinfo", 0);  /* O_RDONLY = 0 */
    if (fd < 0) return -1;

    char buf[4096];
    long nread = jocky_syscall3(SYS_read, fd, (long)buf, sizeof(buf) - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return -1;
    buf[nread] = '\0';

    /* Find "model name" line */
    char* pos = buf;
    while (*pos) {
        if (strncmp(pos, "model name", 10) == 0) {
            pos = strchr(pos, ':');
            if (pos) {
                pos++;
                while (*pos == ' ' || *pos == '\t') pos++;

                char* end = strchr(pos, '\n');
                if (!end) end = pos + strlen(pos);

                size_t len = end - pos;
                if (len > brand_size - 1) len = brand_size - 1;

                strncpy(brand, pos, len);
                brand[len] = '\0';
                return 0;
            }
        }
        pos = strchr(pos, '\n');
        if (!pos) break;
        pos++;
    }

    return -1;
}

int jocky_sysinfo_cpu(jocky_cpu_info_t* out) {
    if (!out) return -1;

    if (cpu_cached) {
        memcpy(out, &cpu_cache, sizeof(jocky_cpu_info_t));
        return 0;
    }

    memset(out, 0, sizeof(jocky_cpu_info_t));

    /* Get online cores via sysconf equivalent */
    long nprocs = jocky_syscall1(SYS_sched_getaffinity, 0);  /* Get current affinity */
    out->cores = nprocs > 0 ? nprocs : 1;
    out->logical_processors = out->cores;

    /* Get CPU brand from /proc/cpuinfo */
    read_proc_cpuinfo(out->brand, sizeof(out->brand));

    /* CPUID for features (x86_64 only) */
    #if defined(__x86_64__) || defined(__i386__)
    uint32_t regs[4];
    __asm__ __volatile__(
        "cpuid"
        : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
        : "0" (1), "2" (0)
    );

    out->has_aes_ni = (regs[2] >> 25) & 1;
    out->has_sse42 = (regs[2] >> 20) & 1;
    out->has_avx = (regs[2] >> 28) & 1;
    #endif

    memcpy(&cpu_cache, out, sizeof(jocky_cpu_info_t));
    cpu_cached = 1;

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

    struct linux_sysinfo si;
    long result = jocky_syscall1(SYS_sysinfo, (long)&si);
    if (result != 0) return -1;

    out->total = si.totalram * (uint64_t)si.mem_unit;
    out->free = si.freeram * (uint64_t)si.mem_unit;
    out->available = out->free + (si.bufferram * (uint64_t)si.mem_unit);
    out->used = out->total - out->free;

    if (out->total > 0) {
        out->percent_used = (int)(100 * out->used / out->total);
    } else {
        out->percent_used = 0;
    }

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

    struct utsname uts;
    long result = jocky_syscall1(SYS_uname, (long)&uts);
    if (result != 0) return -1;

    strncpy(out->os_name, uts.sysname, sizeof(out->os_name) - 1);
    strncpy(out->kernel_release, uts.release, sizeof(out->kernel_release) - 1);
    strncpy(out->kernel_version, uts.version, sizeof(out->kernel_version) - 1);
    strncpy(out->machine, uts.machine, sizeof(out->machine) - 1);

    /* Detect 32 vs 64-bit */
    if (strstr(out->machine, "64") || strcmp(out->machine, "x86_64") == 0 || strcmp(out->machine, "aarch64") == 0) {
        out->bits = 64;
    } else {
        out->bits = 32;
    }

    return 0;
}

uint64_t jocky_sysinfo_uptime(void) {
    struct linux_sysinfo si;
    long result = jocky_syscall1(SYS_sysinfo, (long)&si);
    if (result != 0) return 0;
    return (uint64_t)si.uptime;
}

int jocky_sysinfo_hostname(char* buffer, size_t size) {
    if (!buffer || size == 0) return -1;

    /* sethostname/gethostname not syscalls in modern Linux */
    /* Use /proc/sys/kernel/hostname instead */
    long fd = jocky_syscall2(SYS_open, (long)"/proc/sys/kernel/hostname", 0);
    if (fd < 0) return -1;

    long nread = jocky_syscall3(SYS_read, fd, (long)buffer, size - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return -1;

    buffer[nread] = '\0';
    /* Remove trailing newline if present */
    if (nread > 0 && buffer[nread - 1] == '\n') {
        buffer[nread - 1] = '\0';
    }

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
    /* Get page size via arch_prctl or sysconf equivalent */
    /* For now, return standard 4KB */
    return 4096;
}

int jocky_sysinfo_init(void) {
    jocky_cpu_info_t dummy;
    return jocky_sysinfo_cpu(&dummy);
}

int jocky_sysinfo_load_average(double* out) {
    if (!out) return -1;

    /* Read /proc/loadavg */
    long fd = jocky_syscall2(SYS_open, (long)"/proc/loadavg", 0);
    if (fd < 0) return -1;

    char buf[64];
    long nread = jocky_syscall3(SYS_read, fd, (long)buf, sizeof(buf) - 1);
    jocky_syscall1(SYS_close, fd);

    if (nread <= 0) return -1;
    buf[nread] = '\0';

    /* Parse "X.XX Y.YY Z.ZZ ..." format */
    int count = sscanf(buf, "%lf %lf %lf", &out[0], &out[1], &out[2]);
    return (count == 3) ? 0 : -1;
}
