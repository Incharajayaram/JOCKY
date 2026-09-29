/**
 * JOCKY Runtime Error Handling & Edge Case Tests
 *
 * Comprehensive test suite for error handling, memory leak detection,
 * and safe memory operations.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declarations for testing */
extern void jocky_mem_track_init(void);
extern void* jocky_alloc_tracked(size_t size, const char *file, int line);
extern int32_t jocky_free_tracked(void *ptr, const char *file, int line);
extern void jocky_mem_dump_leaks(void);
extern size_t jocky_mem_total_allocated(void);
extern uint32_t jocky_mem_allocation_count(void);
extern void jocky_mem_track_clear(void);

extern void jocky_safe_free(void **ptr);
extern void* jocky_safe_memcpy(void *dst, const void *src, size_t len);
extern void* jocky_safe_memset(void *ptr, int value, size_t len);
extern void* jocky_safe_memmove(void *dst, const void *src, size_t len);

extern int32_t jocky_safe_strcpy(char *dst, size_t dst_size, const char *src);
extern int32_t jocky_safe_strcat(char *dst, size_t dst_size, const char *src);
extern size_t jocky_safe_strlen(const char *str);
extern int32_t jocky_safe_strcmp(const char *s1, const char *s2);

extern const char* jocky_error_message(int32_t err);
extern void jocky_error_logging_enable(bool enable);

#define JOCKY_SUCCESS 0
#define JOCKY_ERR_NULL_PTR -1
#define JOCKY_ERR_INVALID_SIZE -2
#define JOCKY_ERR_ALLOCATION_FAILED -3
#define JOCKY_ERR_OPERATION_FAILED -4
#define JOCKY_ERR_INVALID_PARAM -5
#define JOCKY_ERR_BUFFER_OVERFLOW -6

typedef struct {
    int passed;
    int failed;
    int skipped;
} test_stats_t;

static test_stats_t g_stats = {0};

static void test_assert(bool condition, const char *test_name) {
    if (condition) {
        printf("[PASS] %s\n", test_name);
        g_stats.passed++;
    } else {
        printf("[FAIL] %s\n", test_name);
        g_stats.failed++;
    }
}

/* ============================================================================
 * Memory Tracking Tests
 * ============================================================================ */

static void test_mem_tracking_basic(void) {
    jocky_mem_track_clear();
    jocky_mem_track_init();

    void *ptr1 = jocky_alloc_tracked(100, "test.c", 10);
    test_assert(ptr1 != NULL, "Allocate 100 bytes");
    test_assert(jocky_mem_allocation_count() == 1, "Allocation count == 1");

    void *ptr2 = jocky_alloc_tracked(50, "test.c", 20);
    test_assert(ptr2 != NULL, "Allocate 50 bytes");
    test_assert(jocky_mem_allocation_count() == 2, "Allocation count == 2");
    test_assert(jocky_mem_total_allocated() == 150, "Total allocated == 150");

    jocky_free_tracked(ptr1, "test.c", 30);
    test_assert(jocky_mem_allocation_count() == 1, "Count after free == 1");
    test_assert(jocky_mem_total_allocated() == 50, "Total after free == 50");

    jocky_free_tracked(ptr2, "test.c", 40);
    test_assert(jocky_mem_allocation_count() == 0, "All freed");
}

static void test_mem_tracking_zero_size(void) {
    jocky_mem_track_clear();
    jocky_mem_track_init();

    void *ptr = jocky_alloc_tracked(0, "test.c", 50);
    test_assert(ptr == NULL, "Zero-size allocation returns NULL");
    test_assert(jocky_mem_allocation_count() == 0, "No tracking for zero-size");
}

static void test_mem_tracking_null_free(void) {
    jocky_mem_track_clear();
    jocky_mem_track_init();

    int32_t result = jocky_free_tracked(NULL, "test.c", 60);
    test_assert(result == JOCKY_SUCCESS, "Freeing NULL returns success");
}

/* ============================================================================
 * Safe Memory Operations Tests
 * ============================================================================ */

static void test_safe_memcpy_basic(void) {
    char src[10] = "hello";
    char dst[10] = {0};

    void *result = jocky_safe_memcpy(dst, src, 5);
    test_assert(result != NULL, "memcpy returns non-NULL");
    test_assert(strcmp(dst, "hello") == 0, "Data copied correctly");
}

static void test_safe_memcpy_null_src(void) {
    char dst[10] = {0};
    void *result = jocky_safe_memcpy(dst, NULL, 5);
    test_assert(result == NULL, "memcpy with NULL src returns NULL");
}

static void test_safe_memcpy_null_dst(void) {
    char src[10] = "hello";
    void *result = jocky_safe_memcpy(NULL, src, 5);
    test_assert(result == NULL, "memcpy with NULL dst returns NULL");
}

static void test_safe_memcpy_zero_len(void) {
    char src[10] = "hello";
    char dst[10] = "world";
    void *result = jocky_safe_memcpy(dst, src, 0);
    test_assert(result != NULL, "memcpy with zero len returns dst");
    test_assert(strcmp(dst, "world") == 0, "No copy on zero len");
}

static void test_safe_memset_basic(void) {
    char buf[10] = "hello";
    void *result = jocky_safe_memset(buf, 0, 10);
    test_assert(result != NULL, "memset returns non-NULL");
    test_assert(buf[0] == 0, "First byte set to 0");
    test_assert(buf[4] == 0, "Fifth byte set to 0");
}

static void test_safe_memset_null(void) {
    void *result = jocky_safe_memset(NULL, 0, 10);
    test_assert(result == NULL, "memset with NULL returns NULL");
}

static void test_safe_memmove_basic(void) {
    char buf[20] = "hello world";
    void *result = jocky_safe_memmove(buf + 5, buf, 5);
    test_assert(result != NULL, "memmove returns non-NULL");
}

static void test_safe_memmove_overlapping(void) {
    char buf[20] = "helloworld";
    jocky_safe_memmove(buf + 2, buf, 5);
    test_assert(buf[0] == 'h', "Original beginning preserved");
}

static void test_safe_free_basic(void) {
    void *ptr = malloc(100);
    test_assert(ptr != NULL, "malloc succeeded");

    jocky_safe_free(&ptr);
    test_assert(ptr == NULL, "Pointer set to NULL after free");
}

static void test_safe_free_null(void) {
    void *ptr = NULL;
    jocky_safe_free(&ptr);
    test_assert(ptr == NULL, "NULL pointer remains NULL");
}

/* ============================================================================
 * String Safety Tests
 * ============================================================================ */

static void test_safe_strlen_valid(void) {
    const char *str = "hello";
    size_t len = jocky_safe_strlen(str);
    test_assert(len == 5, "strlen of 'hello' == 5");
}

static void test_safe_strlen_null(void) {
    size_t len = jocky_safe_strlen(NULL);
    test_assert(len == 0, "strlen of NULL == 0");
}

static void test_safe_strlen_empty(void) {
    const char *str = "";
    size_t len = jocky_safe_strlen(str);
    test_assert(len == 0, "strlen of empty string == 0");
}

static void test_safe_strcpy_basic(void) {
    char dst[20] = {0};
    int32_t result = jocky_safe_strcpy(dst, sizeof(dst), "hello");
    test_assert(result == JOCKY_SUCCESS, "strcpy succeeds");
    test_assert(strcmp(dst, "hello") == 0, "String copied correctly");
}

static void test_safe_strcpy_null_dst(void) {
    int32_t result = jocky_safe_strcpy(NULL, 10, "hello");
    test_assert(result == JOCKY_ERR_NULL_PTR, "NULL dst returns error");
}

static void test_safe_strcpy_null_src(void) {
    char dst[20] = {0};
    int32_t result = jocky_safe_strcpy(dst, sizeof(dst), NULL);
    test_assert(result == JOCKY_ERR_NULL_PTR, "NULL src returns error");
}

static void test_safe_strcpy_zero_size(void) {
    char dst[20] = {0};
    int32_t result = jocky_safe_strcpy(dst, 0, "hello");
    test_assert(result == JOCKY_ERR_NULL_PTR, "Zero dst_size returns error");
}

static void test_safe_strcpy_buffer_overflow(void) {
    char dst[5] = {0};
    int32_t result = jocky_safe_strcpy(dst, sizeof(dst), "hello world");
    test_assert(result == JOCKY_ERR_BUFFER_OVERFLOW, "Buffer overflow detected");
    test_assert(dst[0] == 0, "Destination unchanged on error");
}

static void test_safe_strcat_basic(void) {
    char dst[20] = "hello";
    int32_t result = jocky_safe_strcat(dst, sizeof(dst), " world");
    test_assert(result == JOCKY_SUCCESS, "strcat succeeds");
    test_assert(strcmp(dst, "hello world") == 0, "String concatenated");
}

static void test_safe_strcat_overflow(void) {
    char dst[10] = "hello";
    int32_t result = jocky_safe_strcat(dst, sizeof(dst), " world");
    test_assert(result == JOCKY_ERR_BUFFER_OVERFLOW, "Overflow detected");
}

static void test_safe_strcmp_equal(void) {
    int32_t result = jocky_safe_strcmp("hello", "hello");
    test_assert(result == 0, "strcmp equal returns 0");
}

static void test_safe_strcmp_less(void) {
    int32_t result = jocky_safe_strcmp("a", "b");
    test_assert(result < 0, "strcmp less returns < 0");
}

static void test_safe_strcmp_greater(void) {
    int32_t result = jocky_safe_strcmp("b", "a");
    test_assert(result > 0, "strcmp greater returns > 0");
}

static void test_safe_strcmp_null(void) {
    int32_t result = jocky_safe_strcmp(NULL, "hello");
    test_assert(result == JOCKY_ERR_NULL_PTR, "NULL s1 returns error");

    result = jocky_safe_strcmp("hello", NULL);
    test_assert(result == JOCKY_ERR_NULL_PTR, "NULL s2 returns error");
}

/* ============================================================================
 * Edge Case Tests
 * ============================================================================ */

static void test_edge_empty_string_operations(void) {
    char buf[10] = {0};
    int32_t result = jocky_safe_strcpy(buf, sizeof(buf), "");
    test_assert(result == JOCKY_SUCCESS, "Empty string copy succeeds");
    test_assert(buf[0] == 0, "Buffer is empty");
}

static void test_edge_single_byte_operations(void) {
    char src[2] = "x";
    char dst[2] = {0};
    void *result = jocky_safe_memcpy(dst, src, 1);
    test_assert(result != NULL, "Single byte copy succeeds");
    test_assert(dst[0] == 'x', "Single byte copied");
}

static void test_edge_large_allocation(void) {
    jocky_mem_track_clear();
    jocky_mem_track_init();

    void *ptr = jocky_alloc_tracked(1024 * 1024, "test.c", 100);
    if (ptr != NULL) {
        test_assert(jocky_mem_total_allocated() == 1024 * 1024, "Large allocation tracked");
        jocky_free_tracked(ptr, "test.c", 110);
    } else {
        printf("[SKIP] Large allocation (OOM)\n");
        g_stats.skipped++;
    }
}

static void test_edge_many_small_allocations(void) {
    jocky_mem_track_clear();
    jocky_mem_track_init();

    void *ptrs[100] = {0};
    for (int i = 0; i < 100; i++) {
        ptrs[i] = jocky_alloc_tracked(16, "test.c", 120 + i);
    }

    test_assert(jocky_mem_allocation_count() == 100, "100 allocations tracked");

    for (int i = 0; i < 100; i++) {
        jocky_free_tracked(ptrs[i], "test.c", 220 + i);
    }

    test_assert(jocky_mem_allocation_count() == 0, "All allocations freed");
}

static void test_edge_string_boundary(void) {
    char dst[6] = {0};
    int32_t result = jocky_safe_strcpy(dst, 6, "hello");
    test_assert(result == JOCKY_SUCCESS, "Boundary case (exact fit) succeeds");
    test_assert(strcmp(dst, "hello") == 0, "String copied exactly");

    result = jocky_safe_strcpy(dst, 6, "hello!");
    test_assert(result == JOCKY_ERR_BUFFER_OVERFLOW, "One over boundary fails");
}

/* ============================================================================
 * Error Message Tests
 * ============================================================================ */

static void test_error_messages(void) {
    const char *msg = jocky_error_message(JOCKY_SUCCESS);
    test_assert(msg != NULL && strlen(msg) > 0, "Success message exists");

    msg = jocky_error_message(JOCKY_ERR_NULL_PTR);
    test_assert(msg != NULL && strlen(msg) > 0, "Null pointer message exists");

    msg = jocky_error_message(JOCKY_ERR_BUFFER_OVERFLOW);
    test_assert(msg != NULL && strlen(msg) > 0, "Buffer overflow message exists");
}

/* ============================================================================
 * Main Test Runner
 * ============================================================================ */

int main(void) {
    printf("\n=== JOCKY Runtime Error Handling Tests ===\n\n");

    /* Memory tracking tests */
    printf("--- Memory Tracking ---\n");
    test_mem_tracking_basic();
    test_mem_tracking_zero_size();
    test_mem_tracking_null_free();

    /* Safe memory operation tests */
    printf("\n--- Safe Memory Operations ---\n");
    test_safe_memcpy_basic();
    test_safe_memcpy_null_src();
    test_safe_memcpy_null_dst();
    test_safe_memcpy_zero_len();
    test_safe_memset_basic();
    test_safe_memset_null();
    test_safe_memmove_basic();
    test_safe_memmove_overlapping();
    test_safe_free_basic();
    test_safe_free_null();

    /* String safety tests */
    printf("\n--- String Safety ---\n");
    test_safe_strlen_valid();
    test_safe_strlen_null();
    test_safe_strlen_empty();
    test_safe_strcpy_basic();
    test_safe_strcpy_null_dst();
    test_safe_strcpy_null_src();
    test_safe_strcpy_zero_size();
    test_safe_strcpy_buffer_overflow();
    test_safe_strcat_basic();
    test_safe_strcat_overflow();
    test_safe_strcmp_equal();
    test_safe_strcmp_less();
    test_safe_strcmp_greater();
    test_safe_strcmp_null();

    /* Edge case tests */
    printf("\n--- Edge Cases ---\n");
    test_edge_empty_string_operations();
    test_edge_single_byte_operations();
    test_edge_large_allocation();
    test_edge_many_small_allocations();
    test_edge_string_boundary();

    /* Error message tests */
    printf("\n--- Error Messages ---\n");
    test_error_messages();

    /* Summary */
    printf("\n=== Test Summary ===\n");
    printf("Passed: %d\n", g_stats.passed);
    printf("Failed: %d\n", g_stats.failed);
    printf("Skipped: %d\n", g_stats.skipped);
    printf("Total: %d\n\n", g_stats.passed + g_stats.failed + g_stats.skipped);

    return (g_stats.failed == 0) ? 0 : 1;
}
