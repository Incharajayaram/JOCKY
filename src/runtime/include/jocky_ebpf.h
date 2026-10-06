#ifndef JOCKY_EBPF_H
#define JOCKY_EBPF_H

#include <stdint.h>

int32_t jocky_ebpf_load(const int8_t* prog, int32_t prog_size, int32_t prog_type);
int32_t jocky_ebpf_attach(int32_t prog_fd, int32_t attach_type, int32_t target_fd);
int64_t jocky_ebpf_run(int32_t prog_fd, int8_t* ctx, int32_t ctx_size);
int32_t jocky_ebpf_detach(int32_t prog_fd);
int32_t jocky_ebpf_map_update(int32_t prog_fd, const void* key, const void* value);
int32_t jocky_ebpf_map_lookup(int32_t prog_fd, const void* key, void* out_value);
int32_t jocky_ebpf_unload(int32_t prog_fd);

#endif
