#ifdef _WIN32

#include "../include/jocky_threadpool.h"
#include <windows.h>
#include <stdlib.h>

typedef struct {
    TP_POOL* tp_pool;
    TP_CLEANUP_GROUP* cleanup_group;
    SRWLOCK lock;
    int pending_count;
    int shutdown;
} threadpool_impl_t;

typedef struct {
    jocky_task_callback callback;
    void* context;
    threadpool_impl_t* pool;
} work_item_t;

static VOID CALLBACK work_callback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work);

jocky_threadpool_t jocky_threadpool_create(int num_threads) {
    if (num_threads <= 0) {
        return NULL;
    }

    threadpool_impl_t* pool = malloc(sizeof(threadpool_impl_t));
    if (!pool) return NULL;

    pool->pending_count = 0;
    pool->shutdown = 0;
    InitializeSRWLock(&pool->lock);

    /* Create thread pool */
    pool->tp_pool = CreateThreadpool(NULL);
    if (!pool->tp_pool) {
        free(pool);
        return NULL;
    }

    /* Set pool size */
    SetThreadpoolThreadMinimum(pool->tp_pool, 1);
    SetThreadpoolThreadMaximum(pool->tp_pool, num_threads);

    /* Create cleanup group */
    pool->cleanup_group = CreateThreadpoolCleanupGroup();
    if (!pool->cleanup_group) {
        CloseThreadpool(pool->tp_pool);
        free(pool);
        return NULL;
    }

    return (jocky_threadpool_t)pool;
}

int jocky_threadpool_submit(jocky_threadpool_t pool_handle, jocky_task_callback callback, void* context) {
    if (!pool_handle || !callback) return -1;

    threadpool_impl_t* pool = (threadpool_impl_t*)pool_handle;

    AcquireSRWLockExclusive(&pool->lock);
    if (pool->shutdown) {
        ReleaseSRWLockExclusive(&pool->lock);
        return -1;
    }
    ReleaseSRWLockExclusive(&pool->lock);

    /* Create work item */
    work_item_t* work_item = malloc(sizeof(work_item_t));
    if (!work_item) return -1;

    work_item->callback = callback;
    work_item->context = context;
    work_item->pool = pool;

    /* Create work object */
    PTP_WORK work = CreateThreadpoolWork(work_callback, work_item, NULL);
    if (!work) {
        free(work_item);
        return -1;
    }

    /* Associate with cleanup group so we can wait on them all */
    SetThreadpoolCallbackCleanupGroup((PTP_CALLBACK_ENVIRON)&pool->cleanup_group, pool->cleanup_group, NULL);

    /* Submit work */
    AcquireSRWLockExclusive(&pool->lock);
    pool->pending_count++;
    ReleaseSRWLockExclusive(&pool->lock);

    SubmitThreadpoolWork(work);

    return 0;
}

int jocky_threadpool_wait(jocky_threadpool_t pool_handle) {
    if (!pool_handle) return -1;

    threadpool_impl_t* pool = (threadpool_impl_t*)pool_handle;

    /* Wait for all work items to complete (Windows handles this internally) */
    /* We'll use a simple polling approach since we don't have a direct wait */
    while (1) {
        AcquireSRWLockExclusive(&pool->lock);
        int pending = pool->pending_count;
        ReleaseSRWLockExclusive(&pool->lock);

        if (pending == 0) break;

        SleepEx(1, FALSE);
    }

    return 0;
}

int jocky_threadpool_destroy(jocky_threadpool_t pool_handle) {
    if (!pool_handle) return -1;

    threadpool_impl_t* pool = (threadpool_impl_t*)pool_handle;

    /* Wait for remaining work */
    jocky_threadpool_wait(pool_handle);

    /* Mark as shutdown */
    AcquireSRWLockExclusive(&pool->lock);
    pool->shutdown = 1;
    ReleaseSRWLockExclusive(&pool->lock);

    /* Close cleanup group (waits for all work) */
    CloseThreadpoolCleanupGroup(pool->cleanup_group);

    /* Close thread pool */
    CloseThreadpool(pool->tp_pool);

    free(pool);

    return 0;
}

static VOID CALLBACK work_callback(PTP_CALLBACK_INSTANCE instance, PVOID context, PTP_WORK work) {
    work_item_t* item = (work_item_t*)context;

    if (item && item->callback) {
        item->callback(item->context);
    }

    /* Decrement pending count */
    if (item && item->pool) {
        AcquireSRWLockExclusive(&item->pool->lock);
        item->pool->pending_count--;
        ReleaseSRWLockExclusive(&item->pool->lock);
    }

    /* Close work object */
    if (work) {
        CloseThreadpoolWork(work);
    }

    free(item);
}

#endif /* _WIN32 */
