#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#ifdef __linux__
#include <sys/syscall.h>
#include <linux/bpf.h>
#endif

#include "../../include/jocky_ebpf.h"

#define MAX_EBPF_PROGS 16

static struct {
    int program_fd;
    int map_fd;
    int attached;
    int used;
} prog_table[MAX_EBPF_PROGS];

static int alloc_prog_slot(void) {
    for (int i = 0; i < MAX_EBPF_PROGS; i++) {
        if (!prog_table[i].used) return i;
    }
    return -1;
}

int32_t jocky_ebpf_load(const int8_t* prog, int32_t prog_size, int32_t prog_type)
{
    if (!prog || prog_size <= 0) return -1;

    int slot = alloc_prog_slot();
    if (slot < 0) return -1;

    memset(&prog_table[slot], 0, sizeof(prog_table[slot]));
    prog_table[slot].used = 1;
    prog_table[slot].program_fd = -1;
    prog_table[slot].map_fd = -1;

#ifdef __linux__
    union bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.prog_type = (__u32)prog_type;
    attr.insns = (__u64)(uintptr_t)prog;
    attr.insn_cnt = (__u32)(prog_size / 8);
    attr.license = (__u64)(uintptr_t)"GPL";
    int fd = (int)syscall(SYS_bpf, BPF_PROG_LOAD, &attr, sizeof(attr));
    prog_table[slot].program_fd = fd;
#endif

    return slot;
}

int32_t jocky_ebpf_attach(int32_t prog_fd, int32_t attach_type, int32_t target_fd)
{
    if (prog_fd < 0 || prog_fd >= MAX_EBPF_PROGS || !prog_table[prog_fd].used) return -1;

#ifdef __linux__
    union bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.attach_type = (__u32)attach_type;
    attr.target_fd = target_fd;
    attr.attach_bpf_fd = prog_table[prog_fd].program_fd;
    int ret = (int)syscall(SYS_bpf, BPF_PROG_ATTACH, &attr, sizeof(attr));
    if (ret == 0) {
        prog_table[prog_fd].attached = 1;
        return 1;
    }
    return -1;
#else
    return -1;
#endif
}

int64_t jocky_ebpf_run(int32_t prog_fd, int8_t* ctx, int32_t ctx_size)
{
    if (prog_fd < 0 || prog_fd >= MAX_EBPF_PROGS || !prog_table[prog_fd].used) return -1;
    (void)ctx;
    (void)ctx_size;
    return 0;
}

int32_t jocky_ebpf_unload(int32_t prog_fd)
{
    if (prog_fd < 0 || prog_fd >= MAX_EBPF_PROGS || !prog_table[prog_fd].used) return -1;

#ifdef __linux__
    if (prog_table[prog_fd].attached) {
        prog_table[prog_fd].attached = 0;
    }
    if (prog_table[prog_fd].program_fd >= 0) {
        close(prog_table[prog_fd].program_fd);
    }
    if (prog_table[prog_fd].map_fd >= 0) {
        close(prog_table[prog_fd].map_fd);
    }
#endif

    memset(&prog_table[prog_fd], 0, sizeof(prog_table[prog_fd]));
    return 1;
}

int32_t jocky_ebpf_detach(int32_t prog_fd)
{
    if (prog_fd < 0 || prog_fd >= MAX_EBPF_PROGS || !prog_table[prog_fd].used) return -1;
    prog_table[prog_fd].attached = 0;
    return 1;
}

int32_t jocky_ebpf_map_update(int32_t prog_fd, const void* key, const void* value)
{
    if (prog_fd < 0 || prog_fd >= MAX_EBPF_PROGS || !prog_table[prog_fd].used) return -1;
    if (prog_table[prog_fd].map_fd < 0) return -1;
#ifdef __linux__
    union bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.map_fd = (__u32)prog_table[prog_fd].map_fd;
    attr.key = (__u64)(uintptr_t)key;
    attr.value = (__u64)(uintptr_t)value;
    attr.flags = 0;
    return (int32_t)syscall(SYS_bpf, BPF_MAP_UPDATE_ELEM, &attr, sizeof(attr));
#else
    return -1;
#endif
}

int32_t jocky_ebpf_map_lookup(int32_t prog_fd, const void* key, void* out_value)
{
    if (prog_fd < 0 || prog_fd >= MAX_EBPF_PROGS || !prog_table[prog_fd].used) return -1;
    if (prog_table[prog_fd].map_fd < 0) return -1;
#ifdef __linux__
    union bpf_attr attr;
    memset(&attr, 0, sizeof(attr));
    attr.map_fd = (__u32)prog_table[prog_fd].map_fd;
    attr.key = (__u64)(uintptr_t)key;
    attr.value = (__u64)(uintptr_t)out_value;
    return (int32_t)syscall(SYS_bpf, BPF_MAP_LOOKUP_ELEM, &attr, sizeof(attr));
#else
    return -1;
#endif
}
