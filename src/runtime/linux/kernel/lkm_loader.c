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
