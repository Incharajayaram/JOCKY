#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#ifdef __linux__
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#endif

#include "../../include/jocky_lkm.h"

int32_t jocky_lkm_load(const char* path, const char* module_name)
{
    if (!path || !module_name) return -1;

#ifdef __linux__
    int fd = open(path, O_RDONLY | O_CLOEXEC);
    if (fd < 0) return -1;

    int ret = (int)syscall(SYS_finit_module, fd, "", 0);
    close(fd);
    return (ret == 0) ? 1 : -1;
#else
    return -1;
#endif
}

int32_t jocky_lkm_unload(const char* module_name)
{
    if (!module_name) return -1;

#ifdef __linux__
    int ret = (int)syscall(SYS_delete_module, module_name, O_NONBLOCK);
    return (ret == 0) ? 1 : -1;
#else
    return -1;
#endif
}

int32_t jocky_lkm_hook_syscall(int32_t syscall_num, void* hook_fn)
{
    (void)syscall_num;
    (void)hook_fn;
    return -1;
}

int32_t jocky_lkm_unhook_syscall(int32_t syscall_num)
{
    (void)syscall_num;
    return -1;
}

int32_t jocky_lkm_get_syscall_table(uint64_t* out_addr)
{
    if (!out_addr) return -1;
#ifdef __linux__
    FILE* f = fopen("/proc/kallsyms", "r");
    if (!f) return -1;
    uint64_t addr;
    char type[8], name[128];
    while (fscanf(f, "%lx %s %127s", &addr, type, name) == 3) {
        if (strcmp(name, "sys_call_table") == 0) {
            *out_addr = addr;
            fclose(f);
            return 0;
        }
    }
    fclose(f);
#endif
    return -1;
}
