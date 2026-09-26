#ifndef JOCKY_COMPRESSION_H
#define JOCKY_COMPRESSION_H

#include <stddef.h>
#include <stdint.h>

/* Compression levels */
#define JOCKY_COMPRESS_FAST      1
#define JOCKY_COMPRESS_BALANCED  6
#define JOCKY_COMPRESS_BEST      9

/* Opaque compression context */
typedef void* jocky_compress_ctx_t;

/* ========== COMPRESSION & DECOMPRESSION ========== */

/**
 * Compress data using zlib/deflate.
 * level: JOCKY_COMPRESS_FAST/BALANCED/BEST
 * output_size: allocated buffer size for output
 * Returns 0 on success, -1 on failure.
 * Sets out_size to the actual compressed size.
 */
int jocky_compress(const uint8_t* input, int input_size,
                   uint8_t* output, int output_size, int* out_size,
                   int level);

/**
 * Decompress zlib/deflate data.
 * output_size: allocated buffer size for output
 * Returns 0 on success, -1 on failure.
 * Sets out_size to the actual decompressed size.
 */
int jocky_decompress(const uint8_t* input, int input_size,
                     uint8_t* output, int output_size, int* out_size);

/**
 * Estimate maximum compressed size for input.
 * Useful for allocating output buffer.
 * Returns size needed, 0 on error.
 */
int jocky_compress_bound(int input_size);

/* ========== STREAMING COMPRESSION ========== */

/**
 * Create streaming compression context.
 * level: JOCKY_COMPRESS_FAST/BALANCED/BEST
 * Returns NULL on failure.
 */
jocky_compress_ctx_t jocky_compress_stream_create(int level);

/**
 * Compress a chunk of data in streaming mode.
 * finish: 1 to flush/finalize, 0 for more data coming
 * Returns 0 on success, -1 on failure.
 * Sets out_size to bytes written to output.
 */
int jocky_compress_stream_update(jocky_compress_ctx_t ctx,
                                 const uint8_t* input, int input_size,
                                 uint8_t* output, int output_size,
                                 int* out_size, int finish);

/**
 * Destroy streaming compression context.
 */
void jocky_compress_stream_destroy(jocky_compress_ctx_t ctx);

/* ========== STREAMING DECOMPRESSION ========== */

/**
 * Create streaming decompression context.
 * Returns NULL on failure.
 */
jocky_compress_ctx_t jocky_decompress_stream_create(void);

/**
 * Decompress a chunk of data in streaming mode.
 * finish: 1 to finalize, 0 for more data coming
 * Returns 0 on success, -1 on failure.
 * Sets out_size to bytes written to output.
 */
int jocky_decompress_stream_update(jocky_compress_ctx_t ctx,
                                   const uint8_t* input, int input_size,
                                   uint8_t* output, int output_size,
                                   int* out_size, int finish);

/**
 * Destroy streaming decompression context.
 */
void jocky_decompress_stream_destroy(jocky_compress_ctx_t ctx);

#endif // JOCKY_COMPRESSION_H
