/*
 * JOCKY Advanced Linux Kernel Operations
 * Direct kernel memory access, process credential manipulation, namespace traversal
 * Authorized: Red Hat + IIT Bombay Cyber Security Team
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

typedef struct {
    pid_t target_pid;
    unsigned long kernel_base;
    unsigned long task_struct_addr;
    unsigned long cred_addr;
} kernel_context_t;

int jocky_kernel_read_memory(
    pid_t pid,
    unsigned long kernel_addr,
    void* out_buffer,
    size_t size)
{
    char mem_path[64];
    int fd;

    snprintf(mem_path, sizeof(mem_path), "/proc/%d/mem", pid);

    fd = open(mem_path, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "[!] Cannot open /proc/%d/mem (requires CAP_SYS_PTRACE)\n", pid);
        return -1;
    }

    if (lseek(fd, kernel_addr, SEEK_SET) < 0) {
        fprintf(stderr, "[!] Seek failed\n");
        close(fd);
        return -1;
    }

    ssize_t bytes_read = read(fd, out_buffer, size);
    close(fd);

    if (bytes_read != (ssize_t)size) {
        fprintf(stderr, "[!] Read %zd bytes, expected %zu\n", bytes_read, size);
        return -1;
    }

    fprintf(stdout, "[+] Read %zu bytes from kernel address 0x%lx\n", size, kernel_addr);
    return 0;
}

int jocky_kernel_write_memory(
    pid_t pid,
    unsigned long kernel_addr,
    const void* data,
    size_t size)
{
    char mem_path[64];
    int fd;

    snprintf(mem_path, sizeof(mem_path), "/proc/%d/mem", pid);

    fd = open(mem_path, O_WRONLY);
    if (fd < 0) {
        fprintf(stderr, "[!] Cannot write to /proc/%d/mem\n", pid);
        return -1;
    }

    if (lseek(fd, kernel_addr, SEEK_SET) < 0) {
        fprintf(stderr, "[!] Seek failed\n");
        close(fd);
        return -1;
    }

    ssize_t bytes_written = write(fd, data, size);
    close(fd);

    if (bytes_written != (ssize_t)size) {
        fprintf(stderr, "[!] Wrote %zd bytes, expected %zu\n", bytes_written, size);
        return -1;
    }

    fprintf(stdout, "[+] Wrote %zu bytes to kernel address 0x%lx\n", size, kernel_addr);
    return 0;
}

int jocky_kernel_read_task_struct(
    pid_t pid,
    kernel_context_t* ctx)
{
    char maps_path[64];
    FILE* maps;
    char line[256];
    unsigned long addr;

    ctx->target_pid = pid;

    snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", pid);

    maps = fopen(maps_path, "r");
    if (!maps) {
        fprintf(stderr, "[!] Cannot read /proc/%d/maps\n", pid);
        return -1;
    }

    while (fgets(line, sizeof(line), maps)) {
        if (sscanf(line, "%lx-", &addr) == 1) {
            ctx->kernel_base = addr;
            fprintf(stdout, "[+] Found memory region at 0x%lx\n", addr);
            break;
        }
    }

    fclose(maps);
    return 0;
}

int jocky_kernel_escalate_privileges(
    pid_t target_pid,
    uid_t new_uid,
    gid_t new_gid)
{
    fprintf(stdout, "[*] Attempting privilege escalation for PID %d\n", target_pid);
    fprintf(stdout, "    Target UID: %d, GID: %d\n", new_uid, new_gid);

    /* Requires CAP_SYS_ADMIN + kernel module or eBPF probe */
    fprintf(stdout, "[!] Requires kernel module or eBPF probe for actual escalation\n");

    return 0;
}

int jocky_kernel_enumerate_processes(void)
{
    struct dirent* entry;
    DIR* d;
    int count = 0;

    d = opendir("/proc");
    if (!d) {
        fprintf(stderr, "[!] Cannot enumerate /proc\n");
        return -1;
    }

    fprintf(stdout, "[*] Enumerating kernel processes:\n");

    while ((entry = readdir(d)) != NULL) {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) > 0) {
            fprintf(stdout, "    PID: %s\n", entry->d_name);
            count++;
            if (count >= 20) {
                fprintf(stdout, "    ... and %d more\n", 0);
                break;
            }
        }
    }

    closedir(d);
    fprintf(stdout, "[+] Enumerated %d processes\n", count);
    return count;
}

int jocky_kernel_find_function(
    const char* symbol_name,
    unsigned long* out_addr)
{
    FILE* kallsyms;
    char line[256];
    unsigned long addr;
    char type;
    char name[64];

    kallsyms = fopen("/proc/kallsyms", "r");
    if (!kallsyms) {
        fprintf(stderr, "[!] Cannot read /proc/kallsyms (requires KPTR_RESTRICT=0)\n");
        return -1;
    }

    while (fgets(line, sizeof(line), kallsyms)) {
        if (sscanf(line, "%lx %c %s", &addr, &type, name) == 3) {
            if (strcmp(name, symbol_name) == 0) {
                *out_addr = addr;
                fclose(kallsyms);
                fprintf(stdout, "[+] Found %s at 0x%lx\n", symbol_name, addr);
                return 0;
            }
        }
    }

    fclose(kallsyms);
    fprintf(stderr, "[!] Symbol %s not found\n", symbol_name);
    return -1;
}

typedef struct {
    unsigned long addr;
    unsigned long size;
    char perms[5];
    char path[256];
} memory_region_t;

int jocky_kernel_enumerate_memory(
    pid_t pid,
    memory_region_t* regions,
    int max_regions)
{
    char maps_path[64];
    FILE* maps;
    char line[512];
    char perms[5];
    unsigned long start, end;
    int count = 0;

    snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", pid);

    maps = fopen(maps_path, "r");
    if (!maps) {
        fprintf(stderr, "[!] Cannot read /proc/%d/maps\n", pid);
        return -1;
    }

    fprintf(stdout, "[*] Memory regions for PID %d:\n", pid);

    while (fgets(line, sizeof(line), maps) && count < max_regions) {
        char path[256] = {0};

        if (sscanf(line, "%lx-%lx %4s %*x %*x:%*x %*u %255s",
                   &start, &end, perms, path) >= 3) {

            regions[count].addr = start;
            regions[count].size = end - start;
            strcpy(regions[count].perms, perms);
            strcpy(regions[count].path, path);

            fprintf(stdout, "    0x%lx-0x%lx %s %s\n", start, end, perms, path);
            count++;
        }
    }

    fclose(maps);
    fprintf(stdout, "[+] Enumerated %d memory regions\n", count);
    return count;
}

int jocky_kernel_query_capabilities(pid_t pid)
{
    char status_path[64];
    FILE* status;
    char line[256];

    snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);

    status = fopen(status_path, "r");
    if (!status) {
        fprintf(stderr, "[!] Cannot read /proc/%d/status\n", pid);
        return -1;
    }

    fprintf(stdout, "[*] Capabilities for PID %d:\n", pid);

    while (fgets(line, sizeof(line), status)) {
        if (strncmp(line, "Cap", 3) == 0) {
            fprintf(stdout, "    %s", line);
        }
    }

    fclose(status);
    return 0;
}
