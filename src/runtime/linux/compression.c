#include "../include/jocky_compression.h"
#include <zlib.h>
#include <stdlib.h>
#include <string.h>

int jocky_compress(const uint8_t* input, int input_size,
                   uint8_t* output, int output_size, int* out_size,
                   int level) {
    if (!input || input_size <= 0 || !output || output_size <= 0 || !out_size) {
        return -1;
    }

    if (level != JOCKY_COMPRESS_FAST && level != JOCKY_COMPRESS_BALANCED && level != JOCKY_COMPRESS_BEST) {
        return -1;
    }

    uLongf compressed_size = output_size;
    int result = compress2(output, &compressed_size, (const Bytef*)input, input_size, level);

    if (result != Z_OK) {
        return -1;
    }

    *out_size = (int)compressed_size;
    return 0;
}

int jocky_decompress(const uint8_t* input, int input_size,
                     uint8_t* output, int output_size, int* out_size) {
    if (!input || input_size <= 0 || !output || output_size <= 0 || !out_size) {
        return -1;
    }

    uLongf decompressed_size = output_size;
    int result = uncompress(output, &decompressed_size, (const Bytef*)input, input_size);

    if (result != Z_OK) {
        return -1;
    }

    *out_size = (int)decompressed_size;
    return 0;
}

int jocky_compress_bound(int input_size) {
    if (input_size <= 0) {
        return 0;
    }

    return (int)compressBound(input_size);
}

typedef struct {
    z_stream stream;
    int initialized;
} compress_ctx_impl_t;

jocky_compress_ctx_t jocky_compress_stream_create(int level) {
    if (level != JOCKY_COMPRESS_FAST && level != JOCKY_COMPRESS_BALANCED && level != JOCKY_COMPRESS_BEST) {
        return NULL;
    }

    compress_ctx_impl_t* ctx = (compress_ctx_impl_t*)malloc(sizeof(compress_ctx_impl_t));
    if (!ctx) {
        return NULL;
    }

    memset(&ctx->stream, 0, sizeof(z_stream));
    ctx->initialized = 0;

    if (deflateInit(&ctx->stream, level) != Z_OK) {
        free(ctx);
        return NULL;
    }

    ctx->initialized = 1;
    return (jocky_compress_ctx_t)ctx;
}

int jocky_compress_stream_update(jocky_compress_ctx_t ctx_handle,
                                 const uint8_t* input, int input_size,
                                 uint8_t* output, int output_size,
                                 int* out_size, int finish) {
    if (!ctx_handle || !output || output_size <= 0 || !out_size) {
        return -1;
    }

    compress_ctx_impl_t* ctx = (compress_ctx_impl_t*)ctx_handle;
    if (!ctx->initialized) {
        return -1;
    }

    z_stream* stream = &ctx->stream;
    stream->avail_in = input_size;
    stream->next_in = (Bytef*)input;
    stream->avail_out = output_size;
    stream->next_out = (Bytef*)output;

    int flush = finish ? Z_FINISH : Z_NO_FLUSH;
    int result = deflate(stream, flush);

    if (result == Z_STREAM_ERROR || result == Z_DATA_ERROR) {
        return -1;
    }

    *out_size = output_size - stream->avail_out;
    return 0;
}

void jocky_compress_stream_destroy(jocky_compress_ctx_t ctx_handle) {
    if (!ctx_handle) {
        return;
    }

    compress_ctx_impl_t* ctx = (compress_ctx_impl_t*)ctx_handle;
    if (ctx->initialized) {
        deflateEnd(&ctx->stream);
    }

    free(ctx);
}

typedef struct {
    z_stream stream;
    int initialized;
} decompress_ctx_impl_t;

jocky_compress_ctx_t jocky_decompress_stream_create(void) {
    decompress_ctx_impl_t* ctx = (decompress_ctx_impl_t*)malloc(sizeof(decompress_ctx_impl_t));
    if (!ctx) {
        return NULL;
    }

    memset(&ctx->stream, 0, sizeof(z_stream));
    ctx->initialized = 0;

    if (inflateInit(&ctx->stream) != Z_OK) {
        free(ctx);
        return NULL;
    }

    ctx->initialized = 1;
    return (jocky_compress_ctx_t)ctx;
}

int jocky_decompress_stream_update(jocky_compress_ctx_t ctx_handle,
                                   const uint8_t* input, int input_size,
                                   uint8_t* output, int output_size,
                                   int* out_size, int finish) {
    if (!ctx_handle || !output || output_size <= 0 || !out_size) {
        return -1;
    }

    decompress_ctx_impl_t* ctx = (decompress_ctx_impl_t*)ctx_handle;
    if (!ctx->initialized) {
        return -1;
    }

    z_stream* stream = &ctx->stream;
    stream->avail_in = input_size;
    stream->next_in = (Bytef*)input;
    stream->avail_out = output_size;
    stream->next_out = (Bytef*)output;

    int flush = finish ? Z_FINISH : Z_NO_FLUSH;
    int result = inflate(stream, flush);

    if (result == Z_STREAM_ERROR || result == Z_DATA_ERROR) {
        return -1;
    }

    *out_size = output_size - stream->avail_out;
    return 0;
}

void jocky_decompress_stream_destroy(jocky_compress_ctx_t ctx_handle) {
    if (!ctx_handle) {
        return;
    }

    decompress_ctx_impl_t* ctx = (decompress_ctx_impl_t*)ctx_handle;
    if (ctx->initialized) {
        inflateEnd(&ctx->stream);
    }

    free(ctx);
}
