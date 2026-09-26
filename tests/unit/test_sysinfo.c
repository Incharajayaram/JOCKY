#include "../../src/runtime/include/jocky_sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test 1: CPU information */
int test_cpu_info(void) {
    printf("TEST 1: CPU information retrieval\n");

    jocky_cpu_info_t info;
    if (jocky_sysinfo_cpu(&info) != 0) {
        printf("  FAIL: Could not get CPU info\n");
        return 0;
    }

    if (info.cores <= 0) {
        printf("  FAIL: Invalid core count: %d\n", info.cores);
        return 0;
    }

    if (info.logical_processors < info.cores) {
        printf("  FAIL: Logical processors < cores\n");
        return 0;
    }

    printf("  PASS (cores: %d, logical: %d, brand: %s)\n",
           info.cores, info.logical_processors, info.brand);
    return 1;
}

/* Test 2: CPU core count */
int test_cpu_cores(void) {
    printf("TEST 2: CPU core count query\n");

    int cores = jocky_sysinfo_cpu_cores();
    if (cores <= 0) {
        printf("  FAIL: Invalid core count: %d\n", cores);
        return 0;
    }

    printf("  PASS (cores: %d)\n", cores);
    return 1;
}

/* Test 3: CPU feature detection */
int test_cpu_features(void) {
    printf("TEST 3: CPU feature detection\n");

    int has_avx = jocky_sysinfo_has_avx();
    int has_aes = jocky_sysinfo_has_aes_ni();

    printf("  PASS (AVX: %s, AES-NI: %s)\n",
           has_avx ? "yes" : "no",
           has_aes ? "yes" : "no");
    return 1;
}

/* Test 4: Memory information */
int test_memory_info(void) {
    printf("TEST 4: Memory information retrieval\n");

    jocky_memory_info_t info;
    if (jocky_sysinfo_memory(&info) != 0) {
        printf("  FAIL: Could not get memory info\n");
        return 0;
    }

    if (info.total <= 0) {
        printf("  FAIL: Invalid total memory: %llu\n",
               (unsigned long long)info.total);
        return 0;
    }

    if (info.available > info.total) {
        printf("  FAIL: Available > total\n");
        return 0;
    }

    if (info.percent_used < 0 || info.percent_used > 100) {
        printf("  FAIL: Invalid percentage: %d\n", info.percent_used);
        return 0;
    }

    printf("  PASS (total: %lu MB, available: %lu MB, used: %d%%)\n",
           (unsigned long)(info.total / (1024*1024)),
           (unsigned long)(info.available / (1024*1024)),
           info.percent_used);
    return 1;
}

/* Test 5: Memory total */
int test_memory_total(void) {
    printf("TEST 5: Memory total query\n");

    uint64_t total = jocky_sysinfo_memory_total();
    if (total <= 0) {
        printf("  FAIL: Invalid total: %llu\n", (unsigned long long)total);
        return 0;
    }

    printf("  PASS (total: %lu MB)\n", (unsigned long)(total / (1024*1024)));
    return 1;
}

/* Test 6: Memory available */
int test_memory_available(void) {
    printf("TEST 6: Memory available query\n");

    uint64_t avail = jocky_sysinfo_memory_available();
    if (avail <= 0) {
        printf("  FAIL: Invalid available: %llu\n", (unsigned long long)avail);
        return 0;
    }

    printf("  PASS (available: %lu MB)\n", (unsigned long)(avail / (1024*1024)));
    return 1;
}

/* Test 7: OS information */
int test_osinfo(void) {
    printf("TEST 7: OS information retrieval\n");

    jocky_osinfo_t info;
    if (jocky_sysinfo_osinfo(&info) != 0) {
        printf("  FAIL: Could not get OS info\n");
        return 0;
    }

    if (strlen(info.os_name) == 0) {
        printf("  FAIL: Empty OS name\n");
        return 0;
    }

    if (info.bits != 32 && info.bits != 64) {
        printf("  FAIL: Invalid bits: %d\n", info.bits);
        return 0;
    }

    printf("  PASS (OS: %s %d-bit, kernel: %s, machine: %s)\n",
           info.os_name, info.bits, info.kernel_release, info.machine);
    return 1;
}

/* Test 8: Uptime */
int test_uptime(void) {
    printf("TEST 8: System uptime query\n");

    uint64_t uptime = jocky_sysinfo_uptime();
    if (uptime <= 0) {
        printf("  FAIL: Invalid uptime: %llu\n", (unsigned long long)uptime);
        return 0;
    }

    uint64_t hours = uptime / 3600;
    uint64_t minutes = (uptime % 3600) / 60;

    printf("  PASS (uptime: %lu seconds = %lu h %lu m)\n",
           (unsigned long)uptime, (unsigned long)hours, (unsigned long)minutes);
    return 1;
}

/* Test 9: Hostname */
int test_hostname(void) {
    printf("TEST 9: Hostname query\n");

    char hostname[256];
    if (jocky_sysinfo_hostname(hostname, sizeof(hostname)) != 0) {
        printf("  FAIL: Could not get hostname\n");
        return 0;
    }

    if (strlen(hostname) == 0) {
        printf("  FAIL: Empty hostname\n");
        return 0;
    }

    printf("  PASS (hostname: %s)\n", hostname);
    return 1;
}

/* Test 10: Kernel release */
int test_kernel_release(void) {
    printf("TEST 10: Kernel release query\n");

    char kernel[128];
    if (jocky_sysinfo_kernel_release(kernel, sizeof(kernel)) != 0) {
        printf("  FAIL: Could not get kernel release\n");
        return 0;
    }

    if (strlen(kernel) == 0) {
        printf("  FAIL: Empty kernel release\n");
        return 0;
    }

    printf("  PASS (kernel: %s)\n", kernel);
    return 1;
}

/* Test 11: Architecture */
int test_arch(void) {
    printf("TEST 11: Architecture query\n");

    char arch[64];
    if (jocky_sysinfo_arch(arch, sizeof(arch)) != 0) {
        printf("  FAIL: Could not get architecture\n");
        return 0;
    }

    if (strlen(arch) == 0) {
        printf("  FAIL: Empty architecture\n");
        return 0;
    }

    printf("  PASS (arch: %s)\n", arch);
    return 1;
}

/* Test 12: Page size */
int test_pagesize(void) {
    printf("TEST 12: Page size query\n");

    int pagesize = jocky_sysinfo_pagesize();
    if (pagesize <= 0) {
        printf("  FAIL: Invalid page size: %d\n", pagesize);
        return 0;
    }

    if (pagesize != 4096 && pagesize != 8192 && pagesize != 16384) {
        printf("  WARN: Unusual page size: %d\n", pagesize);
    }

    printf("  PASS (page size: %d bytes)\n", pagesize);
    return 1;
}

/* Test 13: Load average */
int test_load_average(void) {
    printf("TEST 13: Load average query\n");

    double load[3];
    if (jocky_sysinfo_load_average(load) != 0) {
        printf("  FAIL: Could not get load average\n");
        return 0;
    }

    if (load[0] < 0 || load[1] < 0 || load[2] < 0) {
        printf("  FAIL: Negative load average\n");
        return 0;
    }

    printf("  PASS (1m: %.2f, 5m: %.2f, 15m: %.2f)\n", load[0], load[1], load[2]);
    return 1;
}

/* Test 14: Null pointer checks */
int test_null_checks(void) {
    printf("TEST 14: Null pointer error handling\n");

    if (jocky_sysinfo_cpu(NULL) != -1) {
        printf("  FAIL: Should reject NULL CPU\n");
        return 0;
    }

    if (jocky_sysinfo_memory(NULL) != -1) {
        printf("  FAIL: Should reject NULL memory\n");
        return 0;
    }

    if (jocky_sysinfo_osinfo(NULL) != -1) {
        printf("  FAIL: Should reject NULL osinfo\n");
        return 0;
    }

    if (jocky_sysinfo_hostname(NULL, 256) != -1) {
        printf("  FAIL: Should reject NULL hostname buffer\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("JOCKY System Information Tests\n");
    printf("========================================\n\n");

    int passed = 0;
    int total = 0;

    #define RUN_TEST(fn) do { \
        total++; \
        if (fn()) passed++; \
        printf("\n"); \
    } while(0)

    RUN_TEST(test_cpu_info);
    RUN_TEST(test_cpu_cores);
    RUN_TEST(test_cpu_features);
    RUN_TEST(test_memory_info);
    RUN_TEST(test_memory_total);
    RUN_TEST(test_memory_available);
    RUN_TEST(test_osinfo);
    RUN_TEST(test_uptime);
    RUN_TEST(test_hostname);
    RUN_TEST(test_kernel_release);
    RUN_TEST(test_arch);
    RUN_TEST(test_pagesize);
    RUN_TEST(test_load_average);
    RUN_TEST(test_null_checks);

    printf("========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n");

    return (passed == total) ? 0 : 1;
}
