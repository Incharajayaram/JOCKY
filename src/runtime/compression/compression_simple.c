#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int8_t* jocky_compress_data(int8_t* data, uint64_t size) {
    if (!data || size == 0) return NULL;

    uint8_t* output = (uint8_t*)malloc(size + 4);
    if (!output) return NULL;

    memcpy(output + 4, (uint8_t*)data, size);
    *(uint32_t*)output = (uint32_t)size;

    return (int8_t*)output;
}
