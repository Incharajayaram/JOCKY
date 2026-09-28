#include "../include/jocky_threadpool.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    jocky_task_callback callback;
    void* context;
} task_item_t;

typedef struct {
    task_item_t* tasks;
    int capacity;
    int head;
    int tail;
    int count;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    pthread_cond_t empty;
} task_queue_t;

typedef struct {
    pthread_t* threads;
    int num_threads;
    task_queue_t queue;
    int shutdown;
} threadpool_impl_t;

static void* worker_thread(void* arg) {
    threadpool_impl_t* pool = (threadpool_impl_t*)arg;
    task_queue_t* queue = &pool->queue;

    while (1) {
        pthread_mutex_lock(&queue->lock);

        while (queue->count == 0 && !pool->shutdown) {
            pthread_cond_wait(&queue->not_empty, &queue->lock);
        }

        if (pool->shutdown && queue->count == 0) {
            pthread_mutex_unlock(&queue->lock);
            break;
        }

        if (queue->count > 0) {
            task_item_t task = queue->tasks[queue->head];
            queue->head = (queue->head + 1) % queue->capacity;
            queue->count--;

            pthread_cond_broadcast(&queue->empty);
            pthread_mutex_unlock(&queue->lock);

            task.callback(task.context);
        } else {
            pthread_mutex_unlock(&queue->lock);
        }
    }

    return NULL;
}

jocky_threadpool_t jocky_threadpool_create(int num_threads) {
    if (num_threads <= 0) {
        return NULL;
    }

    threadpool_impl_t* pool = (threadpool_impl_t*)malloc(sizeof(threadpool_impl_t));
    if (!pool) {
        return NULL;
    }

    pool->num_threads = num_threads;
    pool->shutdown = 0;

    task_queue_t* queue = &pool->queue;
    queue->capacity = 256;
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    queue->tasks = (task_item_t*)malloc(sizeof(task_item_t) * queue->capacity);

    if (!queue->tasks) {
        free(pool);
        return NULL;
    }

    if (pthread_mutex_init(&queue->lock, NULL) != 0) {
        free(queue->tasks);
        free(pool);
        return NULL;
    }

    if (pthread_cond_init(&queue->not_empty, NULL) != 0) {
        pthread_mutex_destroy(&queue->lock);
        free(queue->tasks);
        free(pool);
        return NULL;
    }

    if (pthread_cond_init(&queue->empty, NULL) != 0) {
        pthread_cond_destroy(&queue->not_empty);
        pthread_mutex_destroy(&queue->lock);
        free(queue->tasks);
        free(pool);
        return NULL;
    }

    pool->threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
    if (!pool->threads) {
        pthread_cond_destroy(&queue->empty);
        pthread_cond_destroy(&queue->not_empty);
        pthread_mutex_destroy(&queue->lock);
        free(queue->tasks);
        free(pool);
        return NULL;
    }

    for (int i = 0; i < num_threads; i++) {
        if (pthread_create(&pool->threads[i], NULL, worker_thread, pool) != 0) {
            for (int j = 0; j < i; j++) {
                pthread_cancel(pool->threads[j]);
            }
            free(pool->threads);
            pthread_cond_destroy(&queue->empty);
            pthread_cond_destroy(&queue->not_empty);
            pthread_mutex_destroy(&queue->lock);
            free(queue->tasks);
            free(pool);
            return NULL;
        }
    }

    return (jocky_threadpool_t)pool;
}

int jocky_threadpool_submit(jocky_threadpool_t pool_handle, jocky_task_callback callback, void* context) {
    if (!pool_handle || !callback) {
        return -1;
    }

    threadpool_impl_t* pool = (threadpool_impl_t*)pool_handle;
    task_queue_t* queue = &pool->queue;

    pthread_mutex_lock(&queue->lock);

    if (pool->shutdown) {
        pthread_mutex_unlock(&queue->lock);
        return -1;
    }

    if (queue->count >= queue->capacity) {
        pthread_mutex_unlock(&queue->lock);
        return -1;
    }

    queue->tasks[queue->tail].callback = callback;
    queue->tasks[queue->tail].context = context;
    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->count++;

    pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->lock);

    return 0;
}

int jocky_threadpool_wait(jocky_threadpool_t pool_handle) {
    if (!pool_handle) {
        return -1;
    }

    threadpool_impl_t* pool = (threadpool_impl_t*)pool_handle;
    task_queue_t* queue = &pool->queue;

    pthread_mutex_lock(&queue->lock);

    while (queue->count > 0) {
        pthread_cond_wait(&queue->empty, &queue->lock);
    }

    pthread_cond_broadcast(&queue->empty);
    pthread_mutex_unlock(&queue->lock);

    return 0;
}

int jocky_threadpool_destroy(jocky_threadpool_t pool_handle) {
    if (!pool_handle) {
        return -1;
    }

    threadpool_impl_t* pool = (threadpool_impl_t*)pool_handle;
    task_queue_t* queue = &pool->queue;

    pthread_mutex_lock(&queue->lock);
    pool->shutdown = 1;
    pthread_cond_broadcast(&queue->not_empty);
    pthread_mutex_unlock(&queue->lock);

    for (int i = 0; i < pool->num_threads; i++) {
        pthread_join(pool->threads[i], NULL);
    }

    pthread_cond_destroy(&queue->empty);
    pthread_cond_destroy(&queue->not_empty);
    pthread_mutex_destroy(&queue->lock);
    free(queue->tasks);
    free(pool->threads);
    free(pool);

    return 0;
}
