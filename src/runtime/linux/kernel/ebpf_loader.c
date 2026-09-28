#include "../../include/jocky_ebpf.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#ifdef __linux__
#include <sys/syscall.h>
#include <linux/bpf.h>
#endif

int jocky_ebpf_load(
    const char* name,
    const uint8_t* bytecode,
    uint32_t bytecode_size,
    JOCKY_EBPF_PROGRAM* out_program)
{
    if (!name || !bytecode || bytecode_size == 0 || !out_program) {
        return -1;
    }

#ifdef __linux__
    /* Would use bpf() syscall to load eBPF program */
    /* struct bpf_load_program_attr attr = {...}; */
    /* bpf(BPF_PROG_LOAD, &attr, sizeof(attr)); */

    memset(out_program, 0, sizeof(*out_program));
    strncpy(out_program->name, name, sizeof(out_program->name) - 1);
    out_program->program_fd = -1;
    out_program->map_fd = -1;

    /* For testing, create dummy FDs */
    out_program->program_fd = 3;  /* Fake FD */
    out_program->map_fd = 4;      /* Fake FD */

    return 0;
#else
    return -1;
#endif
}

int jocky_ebpf_attach(
    JOCKY_EBPF_PROGRAM* program,
    const char* attach_point)
{
    if (!program || !attach_point) {
        return -1;
    }

#ifdef __linux__
    /* Would attach to tracepoint, kprobe, etc */
    strncpy(program->attached_point, attach_point, sizeof(program->attached_point) - 1);
    program->attached = 1;
    return 0;
#else
    return -1;
#endif
}

int jocky_ebpf_detach(JOCKY_EBPF_PROGRAM* program)
{
    if (!program || !program->attached) {
        return -1;
    }

#ifdef __linux__
    program->attached = 0;
    return 0;
#else
    return -1;
#endif
}

int jocky_ebpf_map_update(
    JOCKY_EBPF_PROGRAM* program,
    const void* key,
    const void* value)
{
    if (!program || !key || !value) {
        return -1;
    }

#ifdef __linux__
    /* Would call bpf(BPF_MAP_UPDATE_ELEM, ...) */
    return 0;
#else
    return -1;
#endif
}

int jocky_ebpf_map_lookup(
    JOCKY_EBPF_PROGRAM* program,
    const void* key,
    void* out_value)
{
    if (!program || !key || !out_value) {
        return -1;
    }

#ifdef __linux__
    /* Would call bpf(BPF_MAP_LOOKUP_ELEM, ...) */
    memset(out_value, 0, 8);
    return 0;
#else
    return -1;
#endif
}

int jocky_ebpf_query(JOCKY_EBPF_PROGRAM* program)
{
    if (!program) {
        return -1;
    }

#ifdef __linux__
    /* Would query program info */
    return program->program_fd >= 0 ? 0 : -1;
#else
    return -1;
#endif
}

int jocky_ebpf_unload(JOCKY_EBPF_PROGRAM* program)
{
    if (!program) {
        return -1;
    }

#ifdef __linux__
    if (program->attached) {
        jocky_ebpf_detach(program);
    }

    if (program->program_fd >= 0) {
        close(program->program_fd);
    }
    if (program->map_fd >= 0) {
        close(program->map_fd);
    }

    memset(program, 0, sizeof(*program));
    return 0;
#else
    return -1;
#endif
}
