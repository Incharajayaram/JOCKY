#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

int32_t jocky_lkm_load(const char* path) {
    return 1;
}

int32_t jocky_lkm_unload(const char* name) {
    return 1;
}

int32_t jocky_ebpf_load(const char* bytecode, uint64_t size) {
    if (!bytecode || size == 0) return 0;
    return 1;
}

int32_t jocky_ebpf_attach(const char* event) {
    if (!event) return 0;
    return 1;
}

int8_t* jocky_module_base(const char* module_name) {
    if (!module_name) return NULL;
    return (int8_t*)0x400000;
}

int8_t* jocky_module_load(const char* path) {
    if (!path) return NULL;
    return (int8_t*)0x400000;
}

int32_t jocky_module_unload(int8_t* handle) {
    if (!handle) return 0;
    return 1;
}

void jocky_sleep_and_recheck(void) {
    sleep(1);
}

char* jocky_str_concat(const char* str1, const char* str2) {
    if (!str1 || !str2) return NULL;

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = (char*)malloc(len1 + len2 + 1);

    if (!result) return NULL;

    strcpy(result, str1);
    strcat(result, str2);

    return result;
}

int32_t forensics_flush_arp_cache(void) {
    system("arp -d > /dev/null 2>&1");
    return 1;
}

int32_t forensics_clear_dns_cache(void) {
    system("systemctl restart systemd-resolved 2>/dev/null || true");
    return 1;
}
