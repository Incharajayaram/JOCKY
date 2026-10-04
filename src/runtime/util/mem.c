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
#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

void* jocky_alloc(int64_t size)
{
    if (size <= 0) return NULL;
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (SIZE_T)size);
}

void jocky_free(void* ptr)
{
    if (ptr) HeapFree(GetProcessHeap(), 0, ptr);
}

int8_t* jocky_byovd_new(void)
{
    return (int8_t*)jocky_alloc((int64_t)sizeof(jocky_byovd_t));
}

void jocky_byovd_destroy(int8_t* ctx)
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
    if (p) memset(p, 0, (size_t)size);
    return p;
}

void jocky_free(void* ptr)
{
    free(ptr);
}

#endif /* _WIN32 */
