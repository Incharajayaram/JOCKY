#ifndef JOCKY_THREADPOOL_H
#define JOCKY_THREADPOOL_H

#include <stddef.h>
#include <stdint.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <pthread.h>
#endif

/* Thread pool opaque handle */
typedef void* jocky_threadpool_t;

/* Task callback function: void (*callback)(void* context) */
typedef void (*jocky_task_callback)(void* context);

/* Task structure */
typedef struct {
    jocky_task_callback callback;
    void* context;
} jocky_task_t;

/**
 * Create a thread pool with num_threads worker threads.
 * Returns NULL on failure.
 */
jocky_threadpool_t jocky_threadpool_create(int num_threads);

/**
 * Submit a task to the thread pool.
 * The callback will be called with the provided context.
 * Returns 0 on success, -1 on failure.
 */
int jocky_threadpool_submit(jocky_threadpool_t pool, jocky_task_callback callback, void* context);

/**
 * Wait for all submitted tasks to complete.
 * Blocks until the queue is empty and all workers are idle.
 * Returns 0 on success, -1 on failure.
 */
int jocky_threadpool_wait(jocky_threadpool_t pool);

/**
 * Destroy the thread pool and free all resources.
 * Waits for remaining tasks to complete.
 * Returns 0 on success, -1 on failure.
 */
int jocky_threadpool_destroy(jocky_threadpool_t pool);

#endif // JOCKY_THREADPOOL_H
