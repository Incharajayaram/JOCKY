/**
 * JOCKY Virtual Memory API
 *
 * Cross-platform virtual memory allocation and manipulation.
 * Provides Windows VirtualAlloc-style APIs that work on Linux too.
 */

#ifndef JOCKY_VMEM_H
#define JOCKY_VMEM_H

#include <stdint.h>
#include <stddef.h>

/* Memory allocation types */
#define JOCKY_MEM_COMMIT   0x1000
#define JOCKY_MEM_RESERVE  0x2000
#define JOCKY_MEM_RESET    0x80000

/* Memory protection flags */
#define JOCKY_PAGE_NOACCESS          0x01
#define JOCKY_PAGE_READONLY          0x02
#define JOCKY_PAGE_READWRITE         0x04
#define JOCKY_PAGE_WRITECOPY         0x08
#define JOCKY_PAGE_EXECUTE           0x10
#define JOCKY_PAGE_EXECUTE_READ      0x20
#define JOCKY_PAGE_EXECUTE_READWRITE 0x40
#define JOCKY_PAGE_EXECUTE_WRITECOPY 0x80
#define JOCKY_PAGE_GUARD             0x100
#define JOCKY_PAGE_NOCACHE           0x200

/**
 * Allocate virtual memory at specified address or any address.
 *
 * @param addr Preferred address (NULL for any)
 * @param size Number of bytes to allocate
 * @param type JOCKY_MEM_COMMIT or JOCKY_MEM_RESERVE
 * @param protect Memory protection flags (JOCKY_PAGE_*)
 * @return Pointer to allocated memory, NULL on failure
 *
 * Example:
 *   void* mem = jocky_valloc(NULL, 4096, JOCKY_MEM_COMMIT, JOCKY_PAGE_EXECUTE_READWRITE);
 */
void* jocky_valloc(void* addr, size_t size, uint32_t type, uint32_t protect);

/**
 * Free allocated virtual memory.
 *
 * @param addr Base address from jocky_valloc
 * @param size Size (0 to free entire allocation, size to decommit partial)
 * @param type JOCKY_MEM_RELEASE or JOCKY_MEM_DECOMMIT
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_vfree(void* addr, size_t size, uint32_t type);

/**
 * Change memory protection on allocated region.
 *
 * @param addr Base address
 * @param size Region size
 * @param newprotect New protection flags (JOCKY_PAGE_*)
 * @param oldprotect Output parameter for old protection (can be NULL)
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_vprotect(void* addr, size_t size, uint32_t newprotect, uint32_t* oldprotect);

/**
 * Query information about allocated memory region.
 *
 * @param addr Address to query
 * @param out_base Output: base address of region
 * @param out_size Output: size of region
 * @param out_state Output: JOCKY_MEM_COMMIT or JOCKY_MEM_RESERVE
 * @param out_protect Output: protection flags
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_vquery(void* addr, void** out_base, size_t* out_size,
                     uint32_t* out_state, uint32_t* out_protect);

/**
 * Get page size for current platform.
 *
 * @return Page size in bytes (typically 4096)
 */
size_t jocky_page_size(void);

/**
 * Allocate executable memory (RWX by default).
 * Useful for code injection and shellcode execution.
 *
 * @param size Number of bytes
 * @return Pointer to executable memory, NULL on failure
 */
void* jocky_valloc_exec(size_t size);

/**
 * Fill memory with specific pattern and free (secure wipe).
 *
 * @param addr Address to wipe
 * @param size Size to wipe
 * @param pattern Pattern byte (usually 0x00)
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_vwipe(void* addr, size_t size, uint8_t pattern);

/**
 * Flush instruction cache after code modification.
 *
 * @param addr Address of modified code
 * @param size Size of modified code
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_flush_icache(void* addr, size_t size);

#endif /* JOCKY_VMEM_H */
