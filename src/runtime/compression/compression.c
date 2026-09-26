#include "../include/jocky_compression.h"
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

/* Streaming context wrapper */
typedef struct {
    z_stream stream;
    int mode;  /* 0 = compress, 1 = decompress */
} compress_context_t;

int jocky_compress(const uint8_t* input, int input_size,
                   uint8_t* output, int output_size, int* out_size,
                   int level) {
    if (!input || !output || !out_size || input_size <= 0) {
        return -1;
    }

    if (level < 1 || level > 9) {
        level = JOCKY_COMPRESS_BALANCED;
    }

    z_stream stream;
    memset(&stream, 0, sizeof(stream));

    stream.avail_in = input_size;
    stream.next_in = (unsigned char*)input;
    stream.avail_out = output_size;
    stream.next_out = output;

    if (deflateInit(&stream, level) != Z_OK) {
        return -1;
    }

    int ret = deflate(&stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&stream);
        return -1;
    }

    *out_size = output_size - stream.avail_out;
    deflateEnd(&stream);

    return 0;
}

int jocky_decompress(const uint8_t* input, int input_size,
                     uint8_t* output, int output_size, int* out_size) {
    if (!input || !output || !out_size || input_size <= 0) {
        return -1;
    }

    z_stream stream;
    memset(&stream, 0, sizeof(stream));

    stream.avail_in = input_size;
    stream.next_in = (unsigned char*)input;
    stream.avail_out = output_size;
    stream.next_out = output;

    if (inflateInit(&stream) != Z_OK) {
        return -1;
    }

    int ret = inflate(&stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&stream);
        return -1;
    }

    *out_size = output_size - stream.avail_out;
    inflateEnd(&stream);

    return 0;
}

int jocky_compress_bound(int input_size) {
    if (input_size <= 0) return 0;
    return (int)compressBound(input_size);
}

jocky_compress_ctx_t jocky_compress_stream_create(int level) {
    if (level < 1 || level > 9) {
        level = JOCKY_COMPRESS_BALANCED;
    }

    compress_context_t* ctx = malloc(sizeof(compress_context_t));
    if (!ctx) return NULL;

    memset(&ctx->stream, 0, sizeof(z_stream));
    ctx->mode = 0;  /* compress */

    if (deflateInit(&ctx->stream, level) != Z_OK) {
        free(ctx);
        return NULL;
    }

    return (jocky_compress_ctx_t)ctx;
}

int jocky_compress_stream_update(jocky_compress_ctx_t ctx_handle,
                                 const uint8_t* input, int input_size,
                                 uint8_t* output, int output_size,
                                 int* out_size, int finish) {
    if (!ctx_handle || !output || !out_size || output_size <= 0) {
        return -1;
    }

    compress_context_t* ctx = (compress_context_t*)ctx_handle;

    ctx->stream.avail_in = input_size;
    ctx->stream.next_in = (unsigned char*)input;
    ctx->stream.avail_out = output_size;
    ctx->stream.next_out = output;

    int flush = finish ? Z_FINISH : Z_NO_FLUSH;
    int ret = deflate(&ctx->stream, flush);

    if (ret == Z_STREAM_ERROR || ret == Z_BUF_ERROR) {
        return -1;
    }

    *out_size = output_size - ctx->stream.avail_out;

    return 0;
}

void jocky_compress_stream_destroy(jocky_compress_ctx_t ctx_handle) {
    if (ctx_handle) {
        compress_context_t* ctx = (compress_context_t*)ctx_handle;
        deflateEnd(&ctx->stream);
        free(ctx);
    }
}

jocky_compress_ctx_t jocky_decompress_stream_create(void) {
    compress_context_t* ctx = malloc(sizeof(compress_context_t));
    if (!ctx) return NULL;

    memset(&ctx->stream, 0, sizeof(z_stream));
    ctx->mode = 1;  /* decompress */

    if (inflateInit(&ctx->stream) != Z_OK) {
        free(ctx);
        return NULL;
    }

    return (jocky_compress_ctx_t)ctx;
}

int jocky_decompress_stream_update(jocky_compress_ctx_t ctx_handle,
                                   const uint8_t* input, int input_size,
                                   uint8_t* output, int output_size,
                                   int* out_size, int finish) {
    if (!ctx_handle || !output || !out_size || output_size <= 0) {
        return -1;
    }

    compress_context_t* ctx = (compress_context_t*)ctx_handle;

    ctx->stream.avail_in = input_size;
    ctx->stream.next_in = (unsigned char*)input;
    ctx->stream.avail_out = output_size;
    ctx->stream.next_out = output;

    int flush = finish ? Z_FINISH : Z_NO_FLUSH;
    int ret = inflate(&ctx->stream, flush);

    if (ret == Z_STREAM_ERROR || ret == Z_BUF_ERROR) {
        return -1;
    }

    *out_size = output_size - ctx->stream.avail_out;

    return 0;
}

void jocky_decompress_stream_destroy(jocky_compress_ctx_t ctx_handle) {
    if (ctx_handle) {
        compress_context_t* ctx = (compress_context_t*)ctx_handle;
        inflateEnd(&ctx->stream);
        free(ctx);
    }
}
