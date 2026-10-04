#ifndef JOCKY_RUNTIME_COMMON_COMPRESS_H
#define JOCKY_RUNTIME_COMMON_COMPRESS_H

#include <stdint.h>
#include <stddef.h>

/* Compression/decompression API
 * Platform-agnostic interface for data compression.
 * Implementations use standard compression algorithms (DEFLATE, etc.).
 */

/* Compress data using DEFLATE or similar algorithm
 * Reduces payload size for network transmission or storage.
 * Returns: compressed size on success, -1 on error */
int32_t compress_deflate(const void *input, size_t input_len,
                         void *output, size_t output_len,
                         int32_t compression_level);

/* Decompress DEFLATE-compressed data
 * Restores original data from compressed form.
 * Returns: decompressed size on success, -1 on error */
int32_t decompress_deflate(const void *compressed, size_t compressed_len,
                           void *output, size_t output_len);

/* Generic compress wrapper for multiple algorithms
 * Returns: compressed size on success, -1 on error */
int32_t compress_data(const void *input, size_t input_len,
                      void *output, size_t output_len,
                      const char *algorithm);

/* Generic decompress wrapper for multiple algorithms
 * Returns: decompressed size on success, -1 on error */
int32_t decompress_data(const void *compressed, size_t compressed_len,
                        void *output, size_t output_len,
                        const char *algorithm);

#endif /* JOCKY_RUNTIME_COMMON_COMPRESS_H */
