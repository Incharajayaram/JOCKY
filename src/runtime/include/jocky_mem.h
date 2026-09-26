#ifndef JOCKY_MEM_H
#define JOCKY_MEM_H

#include <stddef.h>
#include <stdint.h>

/* Memory protection flags */
#define JOCKY_PROT_NONE     0
#define JOCKY_PROT_READ     1
#define JOCKY_PROT_WRITE    2
#define JOCKY_PROT_EXEC     4

/* Memory mapping flags */
#define JOCKY_MAP_SHARED    0x01
#define JOCKY_MAP_PRIVATE   0x02
#define JOCKY_MAP_ANON      0x20
#define JOCKY_MAP_FIXED     0x10

/* Seek flags for mmap */
#define JOCKY_SEEK_SET      0
#define JOCKY_SEEK_CUR      1
#define JOCKY_SEEK_END      2

/* ========== MEMORY MAPPING ========== */

/**
 * Allocate memory-mapped region.
 * Returns pointer to mapped region, NULL on error.
 */
void* jocky_mmap(void* addr, size_t length, int prot, int flags, long fd, long offset);

/**
 * Deallocate memory-mapped region.
 * Returns 0 on success, -1 on error.
 */
int jocky_munmap(void* addr, size_t length);

/**
 * Change memory region protection.
 * Returns 0 on success, -1 on error.
 */
int jocky_mprotect(void* addr, size_t length, int prot);

/**
 * Synchronize memory-mapped region to disk.
 * Returns 0 on success, -1 on error.
 */
int jocky_msync(void* addr, size_t length);

/**
 * Allocate anonymous memory (heap-like allocation).
 * Returns pointer to allocated memory, NULL on error.
 */
void* jocky_malloc(size_t size);

/**
 * Deallocate memory previously allocated by jocky_malloc.
 * Returns 0 on success, -1 on error.
 */
int jocky_free(void* ptr);

/**
 * Query page size (for mmap alignment).
 * Returns page size in bytes.
 */
long jocky_pagesize(void);

/* ========== MEMORY INTROSPECTION ========== */

/**
 * Get memory maps for current process.
 * maps: Output array of memory map info
 * max_maps: Maximum number of maps to read
 * Returns number of maps, -1 on error.
 */
int jocky_get_memmaps(void* maps, size_t max_maps);

/**
 * Get memory statistics for current process.
 * Returns RSS in KB, 0 on error.
 */
uint64_t jocky_get_rss(void);

/**
 * Get virtual memory size for current process.
 * Returns VSize in KB, 0 on error.
 */
uint64_t jocky_get_vsize(void);

#endif // JOCKY_MEM_H
