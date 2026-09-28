#include "../include/jocky_compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_compress_decompress() {
    printf("Test: Compress and decompress data\n");

    const char* original = "Hello, World! This is a test of compression.";
    int orig_size = strlen(original);

    int compressed_bound = jocky_compress_bound(orig_size);
    if (compressed_bound <= 0) {
        printf("FAIL: Could not get compress bound\n");
        return;
    }

    uint8_t* compressed = (uint8_t*)malloc(compressed_bound);
    if (!compressed) {
        printf("FAIL: Could not allocate memory\n");
        return;
    }

    int comp_size = 0;
    if (jocky_compress((const uint8_t*)original, orig_size, compressed, compressed_bound, &comp_size, JOCKY_COMPRESS_BALANCED) != 0) {
        printf("FAIL: Could not compress\n");
        free(compressed);
        return;
    }

    uint8_t* decompressed = (uint8_t*)malloc(orig_size);
    if (!decompressed) {
        printf("FAIL: Could not allocate memory\n");
        free(compressed);
        return;
    }

    int decomp_size = 0;
    if (jocky_decompress(compressed, comp_size, decompressed, orig_size, &decomp_size) != 0) {
        printf("FAIL: Could not decompress\n");
        free(decompressed);
        free(compressed);
        return;
    }

    if (decomp_size != orig_size || memcmp(original, decompressed, orig_size) != 0) {
        printf("FAIL: Decompressed data does not match original\n");
        free(decompressed);
        free(compressed);
        return;
    }

    free(decompressed);
    free(compressed);
    printf("PASS\n");
}

void test_compression_levels() {
    printf("Test: Different compression levels\n");

    const char* data = "AAAAAAAAAA BBBBBBBBBB CCCCCCCCCC DDDDDDDDDD EEEEEEEEEE";
    int data_size = strlen(data);

    int levels[] = {JOCKY_COMPRESS_FAST, JOCKY_COMPRESS_BALANCED, JOCKY_COMPRESS_BEST};
    int sizes[3] = {0};

    for (int i = 0; i < 3; i++) {
        int bound = jocky_compress_bound(data_size);
        uint8_t* compressed = (uint8_t*)malloc(bound);

        if (jocky_compress((const uint8_t*)data, data_size, compressed, bound, &sizes[i], levels[i]) != 0) {
            printf("FAIL: Could not compress with level %d\n", levels[i]);
            free(compressed);
            return;
        }

        free(compressed);
    }

    if (sizes[0] == 0 || sizes[1] == 0 || sizes[2] == 0) {
        printf("FAIL: Compression produced zero-sized output\n");
        return;
    }

    printf("PASS (sizes: %d, %d, %d)\n", sizes[0], sizes[1], sizes[2]);
}

void test_invalid_level() {
    printf("Test: Reject invalid compression level\n");

    const char* data = "Test data";
    uint8_t compressed[1024];
    int comp_size = 0;

    if (jocky_compress((const uint8_t*)data, strlen(data), compressed, sizeof(compressed), &comp_size, 99) == 0) {
        printf("FAIL: Should reject invalid level\n");
        return;
    }

    printf("PASS\n");
}

void test_streaming_compress() {
    printf("Test: Streaming compression\n");

    jocky_compress_ctx_t ctx = jocky_compress_stream_create(JOCKY_COMPRESS_BALANCED);
    if (!ctx) {
        printf("FAIL: Could not create compression context\n");
        return;
    }

    const char* data1 = "Hello, ";
    const char* data2 = "World!";

    uint8_t output[1024];
    int out_size = 0;

    if (jocky_compress_stream_update(ctx, (const uint8_t*)data1, strlen(data1), output, sizeof(output), &out_size, 0) != 0) {
        printf("FAIL: Could not compress first chunk\n");
        jocky_compress_stream_destroy(ctx);
        return;
    }

    int total_size = out_size;

    if (jocky_compress_stream_update(ctx, (const uint8_t*)data2, strlen(data2), output + out_size, sizeof(output) - out_size, &out_size, 1) != 0) {
        printf("FAIL: Could not compress second chunk\n");
        jocky_compress_stream_destroy(ctx);
        return;
    }

    total_size += out_size;

    if (total_size <= 0) {
        printf("FAIL: Streaming compression produced zero output\n");
        jocky_compress_stream_destroy(ctx);
        return;
    }

    jocky_compress_stream_destroy(ctx);
    printf("PASS\n");
}

void test_streaming_decompress() {
    printf("Test: Streaming decompression\n");

    const char* original = "Hello, World! This is streaming decompression test.";
    int orig_size = strlen(original);

    int bound = jocky_compress_bound(orig_size);
    uint8_t* compressed = (uint8_t*)malloc(bound);

    int comp_size = 0;
    jocky_compress((const uint8_t*)original, orig_size, compressed, bound, &comp_size, JOCKY_COMPRESS_BALANCED);

    jocky_compress_ctx_t ctx = jocky_decompress_stream_create();
    if (!ctx) {
        printf("FAIL: Could not create decompression context\n");
        free(compressed);
        return;
    }

    uint8_t decompressed[1024];
    int decomp_size = 0;

    if (jocky_decompress_stream_update(ctx, compressed, comp_size, decompressed, sizeof(decompressed), &decomp_size, 1) != 0) {
        printf("FAIL: Could not decompress\n");
        jocky_decompress_stream_destroy(ctx);
        free(compressed);
        return;
    }

    if (decomp_size != orig_size || memcmp(original, decompressed, orig_size) != 0) {
        printf("FAIL: Decompressed data does not match original\n");
        jocky_decompress_stream_destroy(ctx);
        free(compressed);
        return;
    }

    jocky_decompress_stream_destroy(ctx);
    free(compressed);
    printf("PASS\n");
}

void test_null_inputs() {
    printf("Test: Null input handling\n");

    uint8_t buffer[256];
    int out_size = 0;

    if (jocky_compress(NULL, 10, buffer, sizeof(buffer), &out_size, JOCKY_COMPRESS_BALANCED) == 0) {
        printf("FAIL: Should reject null input\n");
        return;
    }

    if (jocky_decompress(NULL, 10, buffer, sizeof(buffer), &out_size) == 0) {
        printf("FAIL: Should reject null input\n");
        return;
    }

    printf("PASS\n");
}

void test_compress_bound() {
    printf("Test: Compression bound calculation\n");

    int sizes[] = {1, 100, 1000, 10000};

    for (int i = 0; i < 4; i++) {
        int bound = jocky_compress_bound(sizes[i]);
        if (bound <= 0 || bound < sizes[i]) {
            printf("FAIL: Invalid bound for size %d\n", sizes[i]);
            return;
        }
    }

    if (jocky_compress_bound(0) != 0) {
        printf("FAIL: Should return 0 for invalid size\n");
        return;
    }

    printf("PASS\n");
}

void test_empty_compression() {
    printf("Test: Compress empty data\n");

    uint8_t output[256];
    int out_size = 0;

    if (jocky_compress(NULL, 0, output, sizeof(output), &out_size, JOCKY_COMPRESS_BALANCED) == 0) {
        printf("FAIL: Should reject empty input\n");
        return;
    }

    printf("PASS\n");
}

int main() {
    printf("Compression Tests\n");
    printf("==================\n\n");

    test_compress_decompress();
    test_compression_levels();
    test_invalid_level();
    test_streaming_compress();
    test_streaming_decompress();
    test_null_inputs();
    test_compress_bound();
    test_empty_compression();

    printf("\n==================\n");
    printf("All tests completed\n");

    return 0;
}
