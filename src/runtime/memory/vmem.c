/**
 * JOCKY Virtual Memory Implementation
 *
 * Cross-platform virtual memory allocation for Windows and Linux.
 */

#include "vmem.h"
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

static DWORD jocky_to_win_protect(uint32_t protect) {
    DWORD wp = PAGE_NOACCESS;

    if (protect & JOCKY_PAGE_READONLY) wp = PAGE_READONLY;
    else if (protect & JOCKY_PAGE_READWRITE) wp = PAGE_READWRITE;
    else if (protect & JOCKY_PAGE_WRITECOPY) wp = PAGE_WRITECOPY;
    else if (protect & JOCKY_PAGE_EXECUTE) wp = PAGE_EXECUTE;
    else if (protect & JOCKY_PAGE_EXECUTE_READ) wp = PAGE_EXECUTE_READ;
    else if (protect & JOCKY_PAGE_EXECUTE_READWRITE) wp = PAGE_EXECUTE_READWRITE;
    else if (protect & JOCKY_PAGE_EXECUTE_WRITECOPY) wp = PAGE_EXECUTE_WRITECOPY;

    if (protect & JOCKY_PAGE_GUARD) wp |= PAGE_GUARD;
    if (protect & JOCKY_PAGE_NOCACHE) wp |= PAGE_NOCACHE;

    return wp;
}

static uint32_t win_to_jocky_protect(DWORD wp) {
    uint32_t protect = 0;

    switch (wp & ~(PAGE_GUARD | PAGE_NOCACHE)) {
        case PAGE_NOACCESS: protect = JOCKY_PAGE_NOACCESS; break;
        case PAGE_READONLY: protect = JOCKY_PAGE_READONLY; break;
        case PAGE_READWRITE: protect = JOCKY_PAGE_READWRITE; break;
        case PAGE_WRITECOPY: protect = JOCKY_PAGE_WRITECOPY; break;
        case PAGE_EXECUTE: protect = JOCKY_PAGE_EXECUTE; break;
        case PAGE_EXECUTE_READ: protect = JOCKY_PAGE_EXECUTE_READ; break;
        case PAGE_EXECUTE_READWRITE: protect = JOCKY_PAGE_EXECUTE_READWRITE; break;
        case PAGE_EXECUTE_WRITECOPY: protect = JOCKY_PAGE_EXECUTE_WRITECOPY; break;
    }

    if (wp & PAGE_GUARD) protect |= JOCKY_PAGE_GUARD;
    if (wp & PAGE_NOCACHE) protect |= JOCKY_PAGE_NOCACHE;

    return protect;
}

void* jocky_valloc(void* addr, size_t size, uint32_t type, uint32_t protect) {
    if (size == 0) return NULL;

    DWORD alloc_type = 0;
    if (type & JOCKY_MEM_COMMIT) alloc_type |= MEM_COMMIT;
    if (type & JOCKY_MEM_RESERVE) alloc_type |= MEM_RESERVE;

    DWORD win_protect = jocky_to_win_protect(protect);

    return VirtualAlloc(addr, size, alloc_type, win_protect);
}

int32_t jocky_vfree(void* addr, size_t size, uint32_t type) {
    if (!addr) return -1;

    DWORD free_type = 0;
    if (type & JOCKY_MEM_RELEASE) {
        free_type = MEM_RELEASE;
        size = 0;  // Must be 0 for MEM_RELEASE
    } else if (type & JOCKY_MEM_RESET) {
        free_type = MEM_DECOMMIT;
    }

    BOOL result = VirtualFree(addr, size, free_type);
    return result ? 0 : -1;
}

int32_t jocky_vprotect(void* addr, size_t size, uint32_t newprotect, uint32_t* oldprotect) {
    if (!addr || size == 0) return -1;

    DWORD win_protect = jocky_to_win_protect(newprotect);
    DWORD old_protect = 0;

    BOOL result = VirtualProtect(addr, size, win_protect, &old_protect);

    if (result && oldprotect) {
        *oldprotect = win_to_jocky_protect(old_protect);
    }

    return result ? 0 : -1;
}

int32_t jocky_vquery(void* addr, void** out_base, size_t* out_size,
                     uint32_t* out_state, uint32_t* out_protect) {
    if (!addr) return -1;

    MEMORY_BASIC_INFORMATION mbi;
    SIZE_T result = VirtualQuery(addr, &mbi, sizeof(mbi));

    if (result == 0) return -1;

    if (out_base) *out_base = mbi.BaseAddress;
    if (out_size) *out_size = mbi.RegionSize;
    if (out_state) *out_state = (mbi.State == MEM_COMMIT) ? JOCKY_MEM_COMMIT : JOCKY_MEM_RESERVE;
    if (out_protect) *out_protect = win_to_jocky_protect(mbi.Protect);

    return 0;
}

size_t jocky_page_size(void) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwPageSize;
}

void* jocky_valloc_exec(size_t size) {
    return jocky_valloc(NULL, size, JOCKY_MEM_COMMIT | JOCKY_MEM_RESERVE,
                       JOCKY_PAGE_EXECUTE_READWRITE);
}

int32_t jocky_vwipe(void* addr, size_t size, uint8_t pattern) {
    if (!addr || size == 0) return -1;

    memset(addr, pattern, size);
    return 0;
}

int32_t jocky_flush_icache(void* addr, size_t size) {
    if (!addr || size == 0) return -1;

    FlushInstructionCache(GetCurrentProcess(), addr, size);
    return 0;
}

#else  /* Linux implementation */

#include <sys/mman.h>
#include <unistd.h>

static int jocky_to_linux_prot(uint32_t protect) {
    int prot = PROT_NONE;

    if (protect & JOCKY_PAGE_READONLY) prot = PROT_READ;
    else if (protect & JOCKY_PAGE_READWRITE) prot = PROT_READ | PROT_WRITE;
    else if (protect & JOCKY_PAGE_EXECUTE) prot = PROT_EXEC;
    else if (protect & JOCKY_PAGE_EXECUTE_READ) prot = PROT_EXEC | PROT_READ;
    else if (protect & JOCKY_PAGE_EXECUTE_READWRITE) prot = PROT_EXEC | PROT_READ | PROT_WRITE;

    return prot;
}

void* jocky_valloc(void* addr, size_t size, uint32_t type, uint32_t protect) {
    if (size == 0) return NULL;

    int prot = jocky_to_linux_prot(protect);
    int flags = MAP_PRIVATE | MAP_ANONYMOUS;

    if (addr) flags |= MAP_FIXED;

    void* result = mmap(addr, size, prot, flags, -1, 0);

    if (result == MAP_FAILED) return NULL;
    return result;
}

int32_t jocky_vfree(void* addr, size_t size, uint32_t type) {
    if (!addr || size == 0) return -1;

    int result = munmap(addr, size);
    return result;
}

int32_t jocky_vprotect(void* addr, size_t size, uint32_t newprotect, uint32_t* oldprotect) {
    if (!addr || size == 0) return -1;

    int prot = jocky_to_linux_prot(newprotect);
    int result = mprotect(addr, size, prot);

    // Linux doesn't provide old protection easily
    if (oldprotect) *oldprotect = newprotect;

    return result;
}

int32_t jocky_vquery(void* addr, void** out_base, size_t* out_size,
                     uint32_t* out_state, uint32_t* out_protect) {
    if (!addr) return -1;

    // Read /proc/self/maps to find memory region
    FILE* fp = fopen("/proc/self/maps", "r");
    if (!fp) return -1;

    char line[256];
    uintptr_t ptr = (uintptr_t)addr;
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        uintptr_t start, end;
        char prot[5];

        if (sscanf(line, "%lx-%lx %s", &start, &end, prot) == 3) {
            if (ptr >= start && ptr < end) {
                if (out_base) *out_base = (void*)start;
                if (out_size) *out_size = end - start;
                if (out_state) *out_state = JOCKY_MEM_COMMIT;

                if (out_protect) {
                    uint32_t p = 0;
                    if (prot[0] == 'r') p |= JOCKY_PAGE_READONLY;
                    if (prot[1] == 'w') p = (p & ~JOCKY_PAGE_READONLY) | JOCKY_PAGE_READWRITE;
                    if (prot[2] == 'x') p |= JOCKY_PAGE_EXECUTE;
                    *out_protect = p;
                }

                found = 1;
                break;
            }
        }
    }

    fclose(fp);
    return found ? 0 : -1;
}

size_t jocky_page_size(void) {
    static size_t page_size = 0;

    if (page_size == 0) {
        page_size = sysconf(_SC_PAGE_SIZE);
        if (page_size <= 0) page_size = 4096;
    }

    return page_size;
}

void* jocky_valloc_exec(size_t size) {
    return jocky_valloc(NULL, size, JOCKY_MEM_COMMIT | JOCKY_MEM_RESERVE,
                       JOCKY_PAGE_EXECUTE_READWRITE);
}

int32_t jocky_vwipe(void* addr, size_t size, uint8_t pattern) {
    if (!addr || size == 0) return -1;

    memset(addr, pattern, size);

    // Use madvise to suggest to kernel
    madvise(addr, size, MADV_DONTNEED);

    return 0;
}

int32_t jocky_flush_icache(void* addr, size_t size) {
    if (!addr || size == 0) return -1;

    // On Linux x86-64, instruction cache is coherent
    // But we can use __builtin_clear_cache for ARM/other architectures
    #if defined(__GNUC__)
    __builtin_clear_cache((char*)addr, (char*)addr + size);
    #endif

    return 0;
}

#endif
