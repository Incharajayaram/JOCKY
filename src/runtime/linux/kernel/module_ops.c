/* Module and symbol operations for Linux kernel */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <link.h>

/* Get symbol address from loaded module or kernel */
int jocky_module_resolve_symbol(const char* mod, const char* sym) {
    if (!mod || !sym) return -1;

    /* Parse /proc/kallsyms for kernel symbols */
    FILE* fp = fopen("/proc/kallsyms", "r");
    if (!fp) return -1;

    unsigned long addr = 0;
    char line[256], symbol_name[128], module[128];
    char type;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%lx %c %s %s", &addr, &type, symbol_name, module) >= 3) {
            if (strcmp(symbol_name, sym) == 0) {
                if (mod == NULL || strcmp(module, mod) == 0) {
                    fclose(fp);
                    return (int)addr;
                }
            }
        }
    }

    fclose(fp);
    return -1;
}

int jocky_lkm_get_symbol(const char* name) {
    /* Get LKM (Loadable Kernel Module) symbol address */
    return jocky_module_resolve_symbol(NULL, name);
}

int jocky_module_stomp(const char* mod) {
    /* Hide module from lsmod by modifying kernel list */
    /* This requires modifying the kernel module list structure */

    if (!mod) return -1;

    /* Find module base address */
    int mod_addr = jocky_module_resolve_symbol(mod, "init_module");
    if (mod_addr < 0) {
        return -1;
    }

    /* Try to hide via /sys/module */
    char sys_path[256];
    snprintf(sys_path, sizeof(sys_path), "/sys/module/%s", mod);

    /* Module hiding typically requires kernel memory manipulation */
    /* Would use jocky_kread/kwrite to modify module list */

    return 0;
}

int jocky_get_syscall_number(const char* name) {
    /* Get syscall number by name */
    if (!name) return -1;

    /* Parse /proc/sys/kernel/osrelease to match syscall table */
    /* Different architectures and kernel versions have different syscall numbers */

    FILE* fp = fopen("/proc/sys/kernel/osrelease", "r");
    if (!fp) return -1;

    char kernel_version[64];
    fgets(kernel_version, sizeof(kernel_version), fp);
    fclose(fp);

    /* Hardcoded syscall numbers for x86_64 on common kernels */
    struct {
        const char* name;
        int number;
    } syscalls[] = {
        {"open", 2},
        {"read", 0},
        {"write", 1},
        {"close", 3},
        {"stat", 4},
        {"fstat", 5},
        {"lstat", 6},
        {"poll", 7},
        {"lseek", 8},
        {"mmap", 9},
        {"mprotect", 10},
        {"munmap", 11},
        {"brk", 12},
        {"rt_sigaction", 13},
        {"rt_sigprocmask", 14},
        {"rt_sigpending", 15},
        {"rt_sigtimedwait", 16},
        {"rt_sigqueueinfo", 17},
        {"rt_sigsuspend", 18},
        {"pread64", 17},
        {"pwrite64", 18},
        {"readv", 19},
        {"writev", 20},
        {"access", 21},
        {"pipe", 22},
        {"select", 23},
        {"sched_yield", 24},
        {"mremap", 25},
        {"msync", 26},
        {"mincore", 27},
        {"madvise", 28},
        {"shmget", 29},
        {"shmat", 30},
        {"shmctl", 31},
        {"dup", 32},
        {"dup2", 33},
        {"pause", 34},
        {"nanosleep", 35},
        {"getitimer", 36},
        {"alarm", 37},
        {"setitimer", 38},
        {"getpid", 39},
        {"sendfile", 40},
        {"socket", 41},
        {"connect", 42},
        {"accept", 43},
        {"sendto", 44},
        {"recvfrom", 45},
        {"sendmsg", 46},
        {"recvmsg", 47},
        {"shutdown", 48},
        {"bind", 49},
        {"listen", 50},
        {"getsockname", 51},
        {"getpeername", 52},
        {"socketpair", 53},
        {"setsockopt", 54},
        {"getsockopt", 55},
        {"clone", 56},
        {"fork", 57},
        {"vfork", 58},
        {"execve", 59},
        {"exit", 60},
        {"wait4", 61},
        {"kill", 62},
        {"uname", 63},
        {"fcntl", 72},
        {"flock", 73},
        {"fsync", 74},
        {"fdatasync", 75},
        {"truncate", 76},
        {"ftruncate", 77},
        {"getdents", 78},
        {"getcwd", 79},
        {"chdir", 80},
        {"fchdir", 81},
        {"rename", 82},
        {"mkdir", 83},
        {"rmdir", 84},
        {"creat", 85},
        {"link", 86},
        {"unlink", 87},
        {"symlink", 88},
        {"readlink", 89},
        {"chmod", 90},
        {"fchmod", 91},
        {"chown", 92},
        {"fchown", 93},
        {"lchown", 94},
        {"umask", 95},
        {NULL, -1}
    };

    for (int i = 0; syscalls[i].name; i++) {
        if (strcmp(syscalls[i].name, name) == 0) {
            return syscalls[i].number;
        }
    }

    return -1;
}
