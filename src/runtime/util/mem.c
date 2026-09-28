/*
 * Runtime Allocators
 *
 * Thin platform wrappers over the system heap, plus opaque constructors for
 * complex runtime structs so Jocky code never needs raw sizeof() values.
 *
 *   jocky_alloc(size)          – zeroed heap allocation
 *   jocky_free(ptr)            – free (no-op on NULL)
 *   jocky_byovd_new()          – allocate + zero a jocky_byovd_t  (Windows)
 *   jocky_byovd_destroy(ctx)   – unload driver + free context      (Windows)
 */

#include "jocky_rt.h"
#include "jocky_error.h"
#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

void* jocky_alloc(int64_t size)
{
    if (size <= 0) return NULL;

    void *ptr = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (SIZE_T)size);
    if (!ptr && size > 0) {
        JOCKY_ERROR_LOG(JOCKY_ERR_ALLOCATION_FAILED, "HeapAlloc returned NULL");
    }
    return ptr;
}

int32_t jocky_alloc_safe(int64_t size, void **out) {
    if (!out) return JOCKY_ERR_NULL_PTR;
    if (size <= 0) return JOCKY_ERR_INVALID_SIZE;

    *out = jocky_alloc(size);
    return (*out) ? JOCKY_SUCCESS : JOCKY_ERR_ALLOCATION_FAILED;
}

void jocky_free(void* ptr)
{
    if (ptr) {
        BOOL result = HeapFree(GetProcessHeap(), 0, ptr);
        if (!result) {
            JOCKY_ERROR_LOG(JOCKY_ERR_OPERATION_FAILED, "HeapFree returned FALSE");
        }
    }
}

void* jocky_byovd_new(void)
{
    return jocky_alloc((int64_t)sizeof(jocky_byovd_t));
}

void jocky_byovd_destroy(void* ctx)
{
    if (!ctx) return;
    jocky_byovd_unload((jocky_byovd_t*)ctx);
    jocky_free(ctx);
}

#else /* Linux / other POSIX */

#include <stdlib.h>

void* jocky_alloc(int64_t size)
{
    if (size <= 0) return NULL;

    void* p = malloc((size_t)size);
    if (!p && size > 0) {
        JOCKY_ERROR_LOG(JOCKY_ERR_ALLOCATION_FAILED, "malloc returned NULL");
        return NULL;
    }

    if (p) memset(p, 0, (size_t)size);
    return p;
}

int32_t jocky_alloc_safe(int64_t size, void **out) {
    if (!out) return JOCKY_ERR_NULL_PTR;
    if (size <= 0) return JOCKY_ERR_INVALID_SIZE;

    *out = jocky_alloc(size);
    return (*out) ? JOCKY_SUCCESS : JOCKY_ERR_ALLOCATION_FAILED;
}

void jocky_free(void* ptr)
{
    free(ptr);
}

#endif /* _WIN32 */
