#ifndef JOCKY_EBPF_H
#define JOCKY_EBPF_H

#include <stdint.h>

/* eBPF program loading and management for kernel-level hooking */

typedef struct {
    char name[256];
    int program_fd;
    int map_fd;
    uint32_t type;  /* BPF_PROG_TYPE_* */
    char attached_point[256];
    int attached;
} JOCKY_EBPF_PROGRAM;

/* Load eBPF program from bytecode */
int jocky_ebpf_load(
    const char* name,
    const uint8_t* bytecode,
    uint32_t bytecode_size,
    JOCKY_EBPF_PROGRAM* out_program);

/* Attach eBPF program to kernel hook point */
int jocky_ebpf_attach(
    JOCKY_EBPF_PROGRAM* program,
    const char* attach_point);

/* Detach eBPF program */
int jocky_ebpf_detach(JOCKY_EBPF_PROGRAM* program);

/* Update eBPF map data */
int jocky_ebpf_map_update(
    JOCKY_EBPF_PROGRAM* program,
    const void* key,
    const void* value);

/* Read eBPF map data */
int jocky_ebpf_map_lookup(
    JOCKY_EBPF_PROGRAM* program,
    const void* key,
    void* out_value);

/* Query program status */
int jocky_ebpf_query(JOCKY_EBPF_PROGRAM* program);

/* Unload eBPF program */
int jocky_ebpf_unload(JOCKY_EBPF_PROGRAM* program);

#endif
