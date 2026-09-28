#include "../include/jocky_threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int value;
    pthread_mutex_t lock;
} test_context_t;

void increment_callback(void* ctx) {
    test_context_t* context = (test_context_t*)ctx;
    pthread_mutex_lock(&context->lock);
    context->value++;
    pthread_mutex_unlock(&context->lock);
}

void print_callback(void* ctx) {
    int* value = (int*)ctx;
    printf("Value: %d\n", *value);
}

void sum_callback(void* ctx) {
    int* sum_ptr = (int*)ctx;
    pthread_mutex_t* sum_lock = (pthread_mutex_t*)(sum_ptr + 1);
    pthread_mutex_lock(sum_lock);
    (*sum_ptr)++;
    pthread_mutex_unlock(sum_lock);
}

void test_create_destroy() {
    printf("Test: Create and destroy thread pool\n");
    jocky_threadpool_t pool = jocky_threadpool_create(4);
    if (!pool) {
        printf("FAIL: Could not create thread pool\n");
        return;
    }
    if (jocky_threadpool_destroy(pool) != 0) {
        printf("FAIL: Could not destroy thread pool\n");
        return;
    }
    printf("PASS\n");
}

void test_invalid_num_threads() {
    printf("Test: Invalid number of threads\n");
    jocky_threadpool_t pool = jocky_threadpool_create(0);
    if (pool != NULL) {
        printf("FAIL: Should reject 0 threads\n");
        jocky_threadpool_destroy(pool);
        return;
    }
    pool = jocky_threadpool_create(-1);
    if (pool != NULL) {
        printf("FAIL: Should reject negative threads\n");
        jocky_threadpool_destroy(pool);
        return;
    }
    printf("PASS\n");
}

void test_submit_and_wait() {
    printf("Test: Submit tasks and wait\n");
    jocky_threadpool_t pool = jocky_threadpool_create(2);
    if (!pool) {
        printf("FAIL: Could not create thread pool\n");
        return;
    }

    test_context_t context;
    context.value = 0;
    pthread_mutex_init(&context.lock, NULL);

    for (int i = 0; i < 10; i++) {
        if (jocky_threadpool_submit(pool, increment_callback, &context) != 0) {
            printf("FAIL: Could not submit task\n");
            jocky_threadpool_destroy(pool);
            pthread_mutex_destroy(&context.lock);
            return;
        }
    }

    if (jocky_threadpool_wait(pool) != 0) {
        printf("FAIL: Wait failed\n");
        jocky_threadpool_destroy(pool);
        pthread_mutex_destroy(&context.lock);
        return;
    }

    if (context.value != 10) {
        printf("FAIL: Expected value 10, got %d\n", context.value);
        jocky_threadpool_destroy(pool);
        pthread_mutex_destroy(&context.lock);
        return;
    }

    jocky_threadpool_destroy(pool);
    pthread_mutex_destroy(&context.lock);
    printf("PASS\n");
}

void test_multiple_submits() {
    printf("Test: Multiple submit and wait cycles\n");
    jocky_threadpool_t pool = jocky_threadpool_create(4);
    if (!pool) {
        printf("FAIL: Could not create thread pool\n");
        return;
    }

    test_context_t context;
    context.value = 0;
    pthread_mutex_init(&context.lock, NULL);

    for (int cycle = 0; cycle < 3; cycle++) {
        for (int i = 0; i < 5; i++) {
            jocky_threadpool_submit(pool, increment_callback, &context);
        }
        jocky_threadpool_wait(pool);
    }

    if (context.value != 15) {
        printf("FAIL: Expected value 15, got %d\n", context.value);
        jocky_threadpool_destroy(pool);
        pthread_mutex_destroy(&context.lock);
        return;
    }

    jocky_threadpool_destroy(pool);
    pthread_mutex_destroy(&context.lock);
    printf("PASS\n");
}

void test_many_tasks() {
    printf("Test: Submit many tasks\n");
    jocky_threadpool_t pool = jocky_threadpool_create(8);
    if (!pool) {
        printf("FAIL: Could not create thread pool\n");
        return;
    }

    test_context_t context;
    context.value = 0;
    pthread_mutex_init(&context.lock, NULL);

    int task_count = 100;
    for (int i = 0; i < task_count; i++) {
        if (jocky_threadpool_submit(pool, increment_callback, &context) != 0) {
            printf("FAIL: Could not submit task %d\n", i);
            jocky_threadpool_destroy(pool);
            pthread_mutex_destroy(&context.lock);
            return;
        }
    }

    jocky_threadpool_wait(pool);

    if (context.value != task_count) {
        printf("FAIL: Expected value %d, got %d\n", task_count, context.value);
        jocky_threadpool_destroy(pool);
        pthread_mutex_destroy(&context.lock);
        return;
    }

    jocky_threadpool_destroy(pool);
    pthread_mutex_destroy(&context.lock);
    printf("PASS\n");
}

void test_single_worker() {
    printf("Test: Single worker thread\n");
    jocky_threadpool_t pool = jocky_threadpool_create(1);
    if (!pool) {
        printf("FAIL: Could not create thread pool\n");
        return;
    }

    test_context_t context;
    context.value = 0;
    pthread_mutex_init(&context.lock, NULL);

    for (int i = 0; i < 20; i++) {
        jocky_threadpool_submit(pool, increment_callback, &context);
    }

    jocky_threadpool_wait(pool);

    if (context.value != 20) {
        printf("FAIL: Expected value 20, got %d\n", context.value);
        jocky_threadpool_destroy(pool);
        pthread_mutex_destroy(&context.lock);
        return;
    }

    jocky_threadpool_destroy(pool);
    pthread_mutex_destroy(&context.lock);
    printf("PASS\n");
}

void test_many_workers() {
    printf("Test: Many worker threads\n");
    jocky_threadpool_t pool = jocky_threadpool_create(16);
    if (!pool) {
        printf("FAIL: Could not create thread pool\n");
        return;
    }

    test_context_t context;
    context.value = 0;
    pthread_mutex_init(&context.lock, NULL);

    int task_count = 64;
    for (int i = 0; i < task_count; i++) {
        jocky_threadpool_submit(pool, increment_callback, &context);
    }

    jocky_threadpool_wait(pool);

    if (context.value != task_count) {
        printf("FAIL: Expected value %d, got %d\n", task_count, context.value);
        jocky_threadpool_destroy(pool);
        pthread_mutex_destroy(&context.lock);
        return;
    }

    jocky_threadpool_destroy(pool);
    pthread_mutex_destroy(&context.lock);
    printf("PASS\n");
}

void test_destroy_with_pending_tasks() {
    printf("Test: Destroy with pending tasks\n");
    jocky_threadpool_t pool = jocky_threadpool_create(2);
    if (!pool) {
        printf("FAIL: Could not create thread pool\n");
        return;
    }

    test_context_t context;
    context.value = 0;
    pthread_mutex_init(&context.lock, NULL);

    for (int i = 0; i < 10; i++) {
        jocky_threadpool_submit(pool, increment_callback, &context);
    }

    if (jocky_threadpool_destroy(pool) != 0) {
        printf("FAIL: Destroy failed\n");
        pthread_mutex_destroy(&context.lock);
        return;
    }

    if (context.value != 10) {
        printf("FAIL: Expected value 10, got %d\n", context.value);
        pthread_mutex_destroy(&context.lock);
        return;
    }

    pthread_mutex_destroy(&context.lock);
    printf("PASS\n");
}

void test_submit_null_callback() {
    printf("Test: Submit null callback\n");
    jocky_threadpool_t pool = jocky_threadpool_create(2);
    if (!pool) {
        printf("FAIL: Could not create thread pool\n");
        return;
    }

    int result = jocky_threadpool_submit(pool, NULL, NULL);
    if (result == 0) {
        printf("FAIL: Should reject null callback\n");
        jocky_threadpool_destroy(pool);
        return;
    }

    jocky_threadpool_destroy(pool);
    printf("PASS\n");
}

void test_operations_after_destroy() {
    printf("Test: Operations after destroy\n");
    jocky_threadpool_t pool = jocky_threadpool_create(2);
    if (!pool) {
        printf("FAIL: Could not create thread pool\n");
        return;
    }

    jocky_threadpool_destroy(pool);

    int result = jocky_threadpool_submit(pool, increment_callback, NULL);
    if (result == 0) {
        printf("FAIL: Should reject submit after destroy\n");
        return;
    }

    printf("PASS\n");
}

int main() {
    printf("Thread Pool Tests\n");
    printf("==================\n\n");

    test_create_destroy();
    test_invalid_num_threads();
    test_submit_and_wait();
    test_multiple_submits();
    test_many_tasks();
    test_single_worker();
    test_many_workers();
    test_destroy_with_pending_tasks();
    test_submit_null_callback();
    test_operations_after_destroy();

    printf("\n==================\n");
    printf("All tests completed\n");

    return 0;
}
