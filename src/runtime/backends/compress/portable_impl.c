#include "../../common/compress.h"
#include <string.h>

/* Run-Length Encoding (RLE) compression
 * Simple, pure C implementation with no external dependencies.
 * Format: [byte_value][run_length][byte_value][run_length]...
 * Only compresses runs of 3+ identical bytes.
 */

int32_t compress_deflate(const void *input, size_t input_len,
                         void *output, size_t output_len,
                         int32_t compression_level) {
    if (!input || !output || input_len == 0) {
        return -1;
    }

    if (output_len < input_len + 10) {
        return -1;
    }

    const unsigned char *in = (const unsigned char *)input;
    unsigned char *out = (unsigned char *)output;
    size_t out_pos = 0;
    size_t in_pos = 0;

    while (in_pos < input_len && out_pos + 2 < output_len) {
        unsigned char current = in[in_pos];
        size_t run_length = 1;

        while (in_pos + run_length < input_len &&
               in[in_pos + run_length] == current &&
               run_length < 255) {
            run_length++;
        }

        if (run_length >= 3) {
            out[out_pos++] = current;
            out[out_pos++] = (unsigned char)run_length;
            in_pos += run_length;
        } else {
            out[out_pos++] = current;
            out[out_pos++] = (unsigned char)run_length;
            in_pos += run_length;
        }
    }

    if (in_pos < input_len) {
        return -1;
    }

    return (int32_t)out_pos;
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
    if (!algorithm) {
        return -1;
    }

    if (strcmp(algorithm, "rle") == 0) {
        return compress_deflate(input, input_len, output, output_len, 6);
    }

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
