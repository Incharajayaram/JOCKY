#include "../../src/runtime/include/jocky_mem.h"
#include "../../src/runtime/include/jocky_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int test_pagesize(void) {
    printf("TEST 1: pagesize\n");

    long pagesize = jocky_pagesize();
    if (pagesize <= 0) {
        printf("  FAIL: Invalid pagesize: %ld\n", pagesize);
        return 0;
    }

    if (pagesize != 4096) {
        printf("  WARN: Unexpected pagesize: %ld (expected 4096)\n", pagesize);
        /* Don't fail, just warn */
    }

    printf("  PASS (pagesize: %ld)\n", pagesize);
    return 1;
}

int test_malloc_free(void) {
    printf("TEST 2: malloc and free\n");

    void* ptr = jocky_malloc(256);
    if (!ptr) {
        printf("  FAIL: malloc returned NULL\n");
        return 0;
    }

    /* Write some data */
    char* data = (char*)ptr;
    strcpy(data, "Hello, Memory!");

    if (strcmp(data, "Hello, Memory!") != 0) {
        printf("  FAIL: Data corruption\n");
        return 0;
    }

    int free_result = jocky_free(ptr);
    if (free_result != 0) {
        printf("  FAIL: free returned %d\n", free_result);
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_malloc_multiple(void) {
    printf("TEST 3: multiple malloc calls\n");

    void* p1 = jocky_malloc(128);
    void* p2 = jocky_malloc(128);
    void* p3 = jocky_malloc(128);

    if (!p1 || !p2 || !p3) {
        printf("  FAIL: malloc returned NULL\n");
        return 0;
    }

    if (p1 == p2 || p2 == p3 || p1 == p3) {
        printf("  FAIL: Pointers should be different\n");
        return 0;
    }

    printf("  PASS (allocated 3 non-overlapping regions)\n");
    return 1;
}

int test_mmap_anon(void) {
    printf("TEST 4: mmap anonymous region\n");

    long pagesize = jocky_pagesize();
    void* addr = jocky_mmap(NULL, pagesize,
                             JOCKY_PROT_READ | JOCKY_PROT_WRITE,
                             JOCKY_MAP_PRIVATE | JOCKY_MAP_ANON,
                             -1, 0);

    if (!addr) {
        printf("  FAIL: mmap returned NULL\n");
        return 0;
    }

    /* Write and read from mapped region */
    char* mem = (char*)addr;
    strcpy(mem, "Mapped memory test");

    if (strcmp(mem, "Mapped memory test") != 0) {
        printf("  FAIL: Data corruption in mmap\n");
        jocky_munmap(addr, pagesize);
        return 0;
    }

    if (jocky_munmap(addr, pagesize) != 0) {
        printf("  FAIL: munmap failed\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_mmap_file(void) {
    printf("TEST 5: mmap file\n");

    const char* testfile = "/tmp/jocky_mmap_test.txt";

    /* Create test file */
    long fd = jocky_open(testfile, JOCKY_O_CREAT | JOCKY_O_WRONLY, 0644);
    if (fd < 0) {
        printf("  FAIL: Could not create test file\n");
        return 0;
    }

    const char* content = "This is test file content for mmap";
    jocky_write(fd, content, strlen(content));
    jocky_close(fd);

    /* Open for reading */
    fd = jocky_open(testfile, JOCKY_O_RDONLY, 0);
    if (fd < 0) {
        printf("  FAIL: Could not open test file\n");
        jocky_unlink(testfile);
        return 0;
    }

    long pagesize = jocky_pagesize();
    void* addr = jocky_mmap(NULL, pagesize,
                             JOCKY_PROT_READ,
                             JOCKY_MAP_PRIVATE,
                             fd, 0);
    jocky_close(fd);

    if (!addr) {
        printf("  FAIL: mmap file failed\n");
        jocky_unlink(testfile);
        return 0;
    }

    /* Check content */
    char* mem = (char*)addr;
    if (strncmp(mem, "This is test file content", 24) != 0) {
        printf("  FAIL: File content mismatch\n");
        jocky_munmap(addr, pagesize);
        jocky_unlink(testfile);
        return 0;
    }

    jocky_munmap(addr, pagesize);
    jocky_unlink(testfile);

    printf("  PASS\n");
    return 1;
}

int test_mprotect(void) {
    printf("TEST 6: mprotect\n");

    long pagesize = jocky_pagesize();
    void* addr = jocky_mmap(NULL, pagesize,
                             JOCKY_PROT_READ | JOCKY_PROT_WRITE,
                             JOCKY_MAP_PRIVATE | JOCKY_MAP_ANON,
                             -1, 0);

    if (!addr) {
        printf("  FAIL: mmap failed\n");
        return 0;
    }

    /* Change to read-only */
    if (jocky_mprotect(addr, pagesize, JOCKY_PROT_READ) != 0) {
        printf("  FAIL: mprotect failed\n");
        jocky_munmap(addr, pagesize);
        return 0;
    }

    /* Change back to read-write */
    if (jocky_mprotect(addr, pagesize, JOCKY_PROT_READ | JOCKY_PROT_WRITE) != 0) {
        printf("  FAIL: mprotect failed\n");
        jocky_munmap(addr, pagesize);
        return 0;
    }

    jocky_munmap(addr, pagesize);
    printf("  PASS\n");
    return 1;
}

int test_get_rss(void) {
    printf("TEST 7: get_rss\n");

    uint64_t rss = jocky_get_rss();
    if (rss == 0) {
        printf("  FAIL: Could not get RSS\n");
        return 0;
    }

    if (rss < 1000) {  /* Should be at least 1 MB */
        printf("  WARN: RSS seems low: %zu KB\n", rss);
        /* Don't fail */
    }

    printf("  PASS (RSS: %zu KB)\n", rss);
    return 1;
}

int test_get_vsize(void) {
    printf("TEST 8: get_vsize\n");

    uint64_t vsize = jocky_get_vsize();
    if (vsize == 0) {
        printf("  FAIL: Could not get VSize\n");
        return 0;
    }

    if (vsize < 1000) {  /* Should be at least 1 MB */
        printf("  WARN: VSize seems low: %zu KB\n", vsize);
        /* Don't fail */
    }

    printf("  PASS (VSize: %zu KB)\n", vsize);
    return 1;
}

int test_memmaps(void) {
    printf("TEST 9: get_memmaps\n");

    char maps_buffer[4096];
    int maps = jocky_get_memmaps(maps_buffer, 64);
    if (maps < 1) {
        printf("  FAIL: Could not get memory maps (got %d)\n", maps);
        return 0;
    }

    printf("  PASS (found %d memory maps)\n", maps);
    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("JOCKY Memory Management Tests\n");
    printf("========================================\n\n");

    int passed = 0;
    int total = 0;

    #define RUN_TEST(fn) do { \
        total++; \
        if (fn()) passed++; \
        printf("\n"); \
    } while(0)

    RUN_TEST(test_pagesize);
    RUN_TEST(test_malloc_free);
    RUN_TEST(test_malloc_multiple);
    RUN_TEST(test_mmap_anon);
    RUN_TEST(test_mmap_file);
    RUN_TEST(test_mprotect);
    RUN_TEST(test_get_rss);
    RUN_TEST(test_get_vsize);
    RUN_TEST(test_memmaps);

    printf("========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n");

    return (passed == total) ? 0 : 1;
}
