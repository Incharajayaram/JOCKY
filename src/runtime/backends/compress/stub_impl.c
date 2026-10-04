#include "../../common/compress.h"

/* Stub compression implementation
 * All functions return -1 (error).
 * Used when compression is not available or disabled.
 */

int32_t compress_deflate(const void *input, size_t input_len,
                         void *output, size_t output_len,
                         int32_t compression_level) {
    (void)input;
    (void)input_len;
    (void)output;
    (void)output_len;
    (void)compression_level;
    return -1;
}

int32_t decompress_deflate(const void *compressed, size_t compressed_len,
                           void *output, size_t output_len) {
    (void)compressed;
    (void)compressed_len;
    (void)output;
    (void)output_len;
    return -1;
}

int32_t compress_data(const void *input, size_t input_len,
                      void *output, size_t output_len,
                      const char *algorithm) {
    (void)input;
    (void)input_len;
    (void)output;
    (void)output_len;
    (void)algorithm;
    return -1;
}

int32_t decompress_data(const void *compressed, size_t compressed_len,
                        void *output, size_t output_len,
                        const char *algorithm) {
    (void)compressed;
    (void)compressed_len;
    (void)output;
    (void)output_len;
    (void)algorithm;
    return -1;
}
