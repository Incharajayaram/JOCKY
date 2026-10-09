#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 * Compatibility shims required by MLIR obfuscation passes on Windows.
 *
 * string_encrypt pass: calls jocky_compress_data / jocky_compress_get_size
 *   to decompress encrypted string literals at runtime.
 *
 * scf_obfuscate pass: injects a ptrace anti-debug check that is Linux-only;
 *   on Windows we return -1 so the check is a no-op.
 */

static int32_t g_last_compress_size = 0;

int8_t* jocky_compress_data(int8_t* data, uint64_t size) {
    if (!data || size == 0) return NULL;
    uint8_t* out = (uint8_t*)malloc((size_t)(size + 4));
    if (!out) return NULL;
    memcpy(out + 4, data, (size_t)size);
    *(uint32_t*)out = (uint32_t)size;
    g_last_compress_size = (int32_t)(size + 4);
    return (int8_t*)out;
}

int32_t jocky_compress_get_size(void) {
    return g_last_compress_size;
}

/* ptrace stub — scf_obfuscate injects this call; always report "not traced" */
long ptrace(int request, ...) {
    (void)request;
    return -1;
}
