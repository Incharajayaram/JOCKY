#ifdef HAVE_ZLIB

#include "../../common/compress.h"
#include <zlib.h>
#include <string.h>

int32_t compress_deflate(const void *input, size_t input_len,
                         void *output, size_t output_len,
                         int32_t compression_level) {
    if (!input || !output || input_len == 0) {
        return -1;
    }

    if (compression_level < 1 || compression_level > 9) {
        compression_level = Z_DEFAULT_COMPRESSION;
    }

    z_stream stream;
    memset(&stream, 0, sizeof(stream));

    stream.avail_in = (uInt)input_len;
    stream.next_in = (unsigned char*)input;
    stream.avail_out = (uInt)output_len;
    stream.next_out = (unsigned char*)output;

    if (deflateInit(&stream, compression_level) != Z_OK) {
        return -1;
    }

    int ret = deflate(&stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        deflateEnd(&stream);
        return -1;
    }

    int32_t compressed_size = (int32_t)(output_len - stream.avail_out);
    deflateEnd(&stream);

    return compressed_size;
}

int32_t decompress_deflate(const void *compressed, size_t compressed_len,
                           void *output, size_t output_len) {
    if (!compressed || !output || compressed_len == 0) {
        return -1;
    }

    z_stream stream;
    memset(&stream, 0, sizeof(stream));

    stream.avail_in = (uInt)compressed_len;
    stream.next_in = (unsigned char*)compressed;
    stream.avail_out = (uInt)output_len;
    stream.next_out = (unsigned char*)output;

    if (inflateInit(&stream) != Z_OK) {
        return -1;
    }

    int ret = inflate(&stream, Z_FINISH);
    if (ret != Z_STREAM_END) {
        inflateEnd(&stream);
        return -1;
    }

    int32_t decompressed_size = (int32_t)(output_len - stream.avail_out);
    inflateEnd(&stream);

    return decompressed_size;
}

int32_t compress_data(const void *input, size_t input_len,
                      void *output, size_t output_len,
                      const char *algorithm) {
    if (!algorithm) {
        return -1;
    }

    if (strcmp(algorithm, "deflate") == 0 || strcmp(algorithm, "zlib") == 0) {
        return compress_deflate(input, input_len, output, output_len, Z_DEFAULT_COMPRESSION);
    }

    return -1;
}

int32_t decompress_data(const void *compressed, size_t compressed_len,
                        void *output, size_t output_len,
                        const char *algorithm) {
    if (!algorithm) {
        return -1;
    }

    if (strcmp(algorithm, "deflate") == 0 || strcmp(algorithm, "zlib") == 0) {
        return decompress_deflate(compressed, compressed_len, output, output_len);
    }

    return -1;
}

#endif /* HAVE_ZLIB */
