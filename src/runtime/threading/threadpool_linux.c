#include "../include/jocky_threadpool.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    jocky_task_t task;
    int valid;  /* 1 if task is valid, 0 if poisoned (shutdown signal) */
} queue_entry_t;

typedef struct {
    queue_entry_t* entries;
    int head;
    int tail;
    int count;
    int capacity;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} task_queue_t;

typedef struct {
    int num_threads;
    pthread_t* threads;
    task_queue_t queue;
    int shutdown;
} threadpool_impl_t;

static void* worker_thread(void* arg);

jocky_threadpool_t jocky_threadpool_create(int num_threads) {
    if (num_threads <= 0) {
        return NULL;
    }

    threadpool_impl_t* pool = malloc(sizeof(threadpool_impl_t));
    if (!pool) return NULL;

    pool->num_threads = num_threads;
    pool->shutdown = 0;

    /* Initialize queue */
    pool->queue.capacity = 256;
    pool->queue.entries = malloc(sizeof(queue_entry_t) * pool->queue.capacity);
    if (!pool->queue.entries) {
        free(pool);
        return NULL;
    }

    pool->queue.head = 0;
    pool->queue.tail = 0;
    pool->queue.count = 0;

    pthread_mutex_init(&pool->queue.lock, NULL);
    pthread_cond_init(&pool->queue.not_empty, NULL);
    pthread_cond_init(&pool->queue.not_full, NULL);

    /* Create worker threads */
    pool->threads = malloc(sizeof(pthread_t) * num_threads);
    if (!pool->threads) {
        free(pool->queue.entries);
        free(pool);
        return NULL;
    }

    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&pool->threads[i], NULL, worker_thread, pool) != 0) {
            /* Cleanup on failure */
            pool->shutdown = 1;
            for (int j = 0; j < i; j++) {
                pthread_join(pool->threads[j], NULL);
            }
            free(pool->threads);
            free(pool->queue.entries);
            free(pool);
            return NULL;
        }
    }

    return (jocky_threadpool_t)pool;
}

int jocky_threadpool_submit(jocky_threadpool_t pool_handle, jocky_task_callback callback, void* context) {
    if (!pool_handle || !callback) return -1;

    threadpool_impl_t* pool = (threadpool_impl_t*)pool_handle;

    pthread_mutex_lock(&pool->queue.lock);

    if (pool->shutdown) {
        pthread_mutex_unlock(&pool->queue.lock);
        return -1;
    }

    /* Wait if queue is full */
    while (pool->queue.count >= pool->queue.capacity) {
        pthread_cond_wait(&pool->queue.not_full, &pool->queue.lock);
        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->queue.lock);
            return -1;
        }
    }

    /* Add task to queue */
    pool->queue.entries[pool->queue.tail].task.callback = callback;
    pool->queue.entries[pool->queue.tail].task.context = context;
    pool->queue.entries[pool->queue.tail].valid = 1;

    pool->queue.tail = (pool->queue.tail + 1) % pool->queue.capacity;
    pool->queue.count++;

    pthread_cond_signal(&pool->queue.not_empty);
    pthread_mutex_unlock(&pool->queue.lock);

    return 0;
}

int jocky_threadpool_wait(jocky_threadpool_t pool_handle) {
    if (!pool_handle) return -1;

    threadpool_impl_t* pool = (threadpool_impl_t*)pool_handle;

    pthread_mutex_lock(&pool->queue.lock);
    while (pool->queue.count > 0) {
        pthread_cond_wait(&pool->queue.not_full, &pool->queue.lock);
    }
    pthread_mutex_unlock(&pool->queue.lock);

    return 0;
}

int jocky_threadpool_destroy(jocky_threadpool_t pool_handle) {
    if (!pool_handle) return -1;

    threadpool_impl_t* pool = (threadpool_impl_t*)pool_handle;

    /* Wait for remaining tasks */
    jocky_threadpool_wait(pool_handle);

    /* Send shutdown signal (poison pills) */
    pthread_mutex_lock(&pool->queue.lock);
    pool->shutdown = 1;

    for (int i = 0; i < pool->num_threads; i++) {
        while (pool->queue.count >= pool->queue.capacity) {
            pthread_cond_wait(&pool->queue.not_full, &pool->queue.lock);
        }
        pool->queue.entries[pool->queue.tail].valid = 0;  /* Poison pill */
        pool->queue.tail = (pool->queue.tail + 1) % pool->queue.capacity;
        pool->queue.count++;
    }

    pthread_cond_broadcast(&pool->queue.not_empty);
    pthread_mutex_unlock(&pool->queue.lock);

    /* Wait for all threads to finish */
    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    /* Cleanup */
    pthread_mutex_destroy(&pool->queue.lock);
    pthread_cond_destroy(&pool->queue.not_empty);
    pthread_cond_destroy(&pool->queue.not_full);

    free(pool->threads);
    free(pool->queue.entries);
    free(pool);

    return 0;
}

static void* worker_thread(void* arg) {
    threadpool_impl_t* pool = (threadpool_impl_t*)arg;

    while (1) {
        pthread_mutex_lock(&pool->queue.lock);

        /* Wait for work */
        while (pool->queue.count == 0) {
            pthread_cond_wait(&pool->queue.not_empty, &pool->queue.lock);
        }

        /* Get task from queue */
        queue_entry_t entry = pool->queue.entries[pool->queue.head];
        pool->queue.head = (pool->queue.head + 1) % pool->queue.capacity;
        pool->queue.count--;

        /* Signal that space is available */
        pthread_cond_signal(&pool->queue.not_full);
        pthread_mutex_unlock(&pool->queue.lock);

        /* Check for shutdown signal */
        if (!entry.valid) {
            break;  /* Worker exits */
        }

        /* Execute task */
        entry.task.callback(entry.task.context);
    }

    return NULL;
}
