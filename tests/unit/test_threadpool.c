#include "../../src/runtime/include/jocky_threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdatomic.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <pthread.h>
#endif

/* Test counters */
static _Atomic(int) executed_count = 0;
static _Atomic(int) execution_order[256];
static _Atomic(int) next_order_idx = 0;

/* Test callback: simple counter increment */
static void increment_callback(void* context) {
    atomic_fetch_add(&executed_count, 1);
}

/* Test callback: with context value */
static void context_callback(void* context) {
    int* val = (int*)context;
    if (val) {
        atomic_fetch_add(&executed_count, *val);
    }
}

/* Test callback: track execution order */
static void order_tracking_callback(void* context) {
    int* order_val = (int*)context;
    int idx = atomic_fetch_add(&next_order_idx, 1);
    if (idx < 256) {
        atomic_store(&execution_order[idx], *order_val);
    }
    atomic_fetch_add(&executed_count, 1);
}

/* Test 1: Create and destroy empty pool */
int test_create_destroy_empty(void) {
    printf("TEST 1: Create and destroy empty pool\n");

    jocky_threadpool_t pool = jocky_threadpool_create(4);
    if (!pool) {
        printf("  FAIL: Could not create pool\n");
        return 0;
    }

    int result = jocky_threadpool_destroy(pool);
    if (result != 0) {
        printf("  FAIL: Could not destroy pool\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

/* Test 2: Invalid thread count */
int test_invalid_thread_count(void) {
    printf("TEST 2: Invalid thread count (0 and negative)\n");

    if (jocky_threadpool_create(0) != NULL) {
        printf("  FAIL: Should reject 0 threads\n");
        return 0;
    }

    if (jocky_threadpool_create(-1) != NULL) {
        printf("  FAIL: Should reject negative threads\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

/* Test 3: Submit and wait single task */
int test_single_task(void) {
    printf("TEST 3: Submit and wait single task\n");

    executed_count = 0;
    jocky_threadpool_t pool = jocky_threadpool_create(2);
    if (!pool) {
        printf("  FAIL: Could not create pool\n");
        return 0;
    }

    if (jocky_threadpool_submit(pool, increment_callback, NULL) != 0) {
        printf("  FAIL: Could not submit task\n");
        jocky_threadpool_destroy(pool);
        return 0;
    }

    if (jocky_threadpool_wait(pool) != 0) {
        printf("  FAIL: Wait failed\n");
        jocky_threadpool_destroy(pool);
        return 0;
    }

    if (atomic_load(&executed_count) != 1) {
        printf("  FAIL: Expected 1 execution, got %d\n", atomic_load(&executed_count));
        jocky_threadpool_destroy(pool);
        return 0;
    }

    jocky_threadpool_destroy(pool);
    printf("  PASS\n");
    return 1;
}

/* Test 4: Submit multiple tasks */
int test_multiple_tasks(void) {
    printf("TEST 4: Submit multiple tasks (100 tasks, 4 threads)\n");

    executed_count = 0;
    jocky_threadpool_t pool = jocky_threadpool_create(4);
    if (!pool) {
        printf("  FAIL: Could not create pool\n");
        return 0;
    }

    /* Submit 100 tasks */
    for (int i = 0; i < 100; i++) {
        if (jocky_threadpool_submit(pool, increment_callback, NULL) != 0) {
            printf("  FAIL: Could not submit task %d\n", i);
            jocky_threadpool_destroy(pool);
            return 0;
        }
    }

    if (jocky_threadpool_wait(pool) != 0) {
        printf("  FAIL: Wait failed\n");
        jocky_threadpool_destroy(pool);
        return 0;
    }

    int count = atomic_load(&executed_count);
    if (count != 100) {
        printf("  FAIL: Expected 100 executions, got %d\n", count);
        jocky_threadpool_destroy(pool);
        return 0;
    }

    jocky_threadpool_destroy(pool);
    printf("  PASS\n");
    return 1;
}

/* Test 5: Task with context data */
int test_task_context(void) {
    printf("TEST 5: Task with context data\n");

    executed_count = 0;
    jocky_threadpool_t pool = jocky_threadpool_create(2);
    if (!pool) {
        printf("  FAIL: Could not create pool\n");
        return 0;
    }

    /* Submit tasks with values to add */
    int values[10];
    for (int i = 0; i < 10; i++) {
        values[i] = i + 1;
        if (jocky_threadpool_submit(pool, context_callback, &values[i]) != 0) {
            printf("  FAIL: Could not submit task %d\n", i);
            jocky_threadpool_destroy(pool);
            return 0;
        }
    }

    if (jocky_threadpool_wait(pool) != 0) {
        printf("  FAIL: Wait failed\n");
        jocky_threadpool_destroy(pool);
        return 0;
    }

    /* Expected sum: 1+2+3+...+10 = 55 */
    int count = atomic_load(&executed_count);
    if (count != 55) {
        printf("  FAIL: Expected sum 55, got %d\n", count);
        jocky_threadpool_destroy(pool);
        return 0;
    }

    jocky_threadpool_destroy(pool);
    printf("  PASS\n");
    return 1;
}

/* Test 6: Submit after wait (reuse pool) */
int test_reuse_pool(void) {
    printf("TEST 6: Reuse pool (submit, wait, submit, wait)\n");

    executed_count = 0;
    jocky_threadpool_t pool = jocky_threadpool_create(2);
    if (!pool) {
        printf("  FAIL: Could not create pool\n");
        return 0;
    }

    /* First batch */
    for (int i = 0; i < 10; i++) {
        jocky_threadpool_submit(pool, increment_callback, NULL);
    }
    jocky_threadpool_wait(pool);

    int count1 = atomic_load(&executed_count);

    /* Second batch */
    for (int i = 0; i < 20; i++) {
        jocky_threadpool_submit(pool, increment_callback, NULL);
    }
    jocky_threadpool_wait(pool);

    int count2 = atomic_load(&executed_count);

    if (count1 != 10 || count2 != 30) {
        printf("  FAIL: Expected 10 then 30 total, got %d then %d\n", count1, count2);
        jocky_threadpool_destroy(pool);
        return 0;
    }

    jocky_threadpool_destroy(pool);
    printf("  PASS\n");
    return 1;
}

/* Test 7: Single thread pool */
int test_single_thread(void) {
    printf("TEST 7: Single thread pool\n");

    executed_count = 0;
    jocky_threadpool_t pool = jocky_threadpool_create(1);
    if (!pool) {
        printf("  FAIL: Could not create pool\n");
        return 0;
    }

    for (int i = 0; i < 50; i++) {
        jocky_threadpool_submit(pool, increment_callback, NULL);
    }

    jocky_threadpool_wait(pool);

    int count = atomic_load(&executed_count);
    if (count != 50) {
        printf("  FAIL: Expected 50 executions, got %d\n", count);
        jocky_threadpool_destroy(pool);
        return 0;
    }

    jocky_threadpool_destroy(pool);
    printf("  PASS\n");
    return 1;
}

/* Test 8: Invalid submit (NULL callback) */
int test_null_callback(void) {
    printf("TEST 8: Invalid submit (NULL callback)\n");

    jocky_threadpool_t pool = jocky_threadpool_create(2);
    if (!pool) {
        printf("  FAIL: Could not create pool\n");
        return 0;
    }

    int result = jocky_threadpool_submit(pool, NULL, NULL);

    jocky_threadpool_destroy(pool);

    if (result == 0) {
        printf("  FAIL: Should reject NULL callback\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

/* Test 9: Submit after destroy (should fail) */
int test_submit_after_destroy(void) {
    printf("TEST 9: Submit after destroy (should fail)\n");

    jocky_threadpool_t pool = jocky_threadpool_create(2);
    if (!pool) {
        printf("  FAIL: Could not create pool\n");
        return 0;
    }

    jocky_threadpool_destroy(pool);

    int result = jocky_threadpool_submit(pool, increment_callback, NULL);

    if (result == 0) {
        printf("  FAIL: Should not allow submit after destroy\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

/* Test 10: Multiple sequential waits */
int test_multiple_waits(void) {
    printf("TEST 10: Multiple sequential waits\n");

    executed_count = 0;
    jocky_threadpool_t pool = jocky_threadpool_create(2);
    if (!pool) {
        printf("  FAIL: Could not create pool\n");
        return 0;
    }

    /* Submit batch 1 */
    for (int i = 0; i < 5; i++) {
        jocky_threadpool_submit(pool, increment_callback, NULL);
    }

    if (jocky_threadpool_wait(pool) != 0) {
        printf("  FAIL: First wait failed\n");
        jocky_threadpool_destroy(pool);
        return 0;
    }

    /* Wait again (should be no-op) */
    if (jocky_threadpool_wait(pool) != 0) {
        printf("  FAIL: Second wait failed\n");
        jocky_threadpool_destroy(pool);
        return 0;
    }

    int count = atomic_load(&executed_count);
    if (count != 5) {
        printf("  FAIL: Expected 5 executions, got %d\n", count);
        jocky_threadpool_destroy(pool);
        return 0;
    }

    jocky_threadpool_destroy(pool);
    printf("  PASS\n");
    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("JOCKY Thread Pool Tests\n");
    printf("========================================\n\n");

    int passed = 0;
    int total = 0;

    #define RUN_TEST(fn) do { \
        total++; \
        if (fn()) passed++; \
        printf("\n"); \
    } while(0)

    RUN_TEST(test_create_destroy_empty);
    RUN_TEST(test_invalid_thread_count);
    RUN_TEST(test_single_task);
    RUN_TEST(test_multiple_tasks);
    RUN_TEST(test_task_context);
    RUN_TEST(test_reuse_pool);
    RUN_TEST(test_single_thread);
    RUN_TEST(test_null_callback);
    RUN_TEST(test_submit_after_destroy);
    RUN_TEST(test_multiple_waits);

    printf("========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n");

    return (passed == total) ? 0 : 1;
}
