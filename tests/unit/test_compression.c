#include "../../src/runtime/include/jocky_compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test 1: Basic compress/decompress */
int test_basic_compress_decompress(void) {
    printf("TEST 1: Basic compress/decompress\n");

    const uint8_t plaintext[] = "Hello, World! Hello, World! Hello, World!";
    int plaintext_size = strlen((const char*)plaintext);

    int compressed_size_max = jocky_compress_bound(plaintext_size);
    if (compressed_size_max <= 0) {
        printf("  FAIL: compress_bound returned %d\n", compressed_size_max);
        return 0;
    }

    uint8_t* compressed = malloc(compressed_size_max);
    if (!compressed) {
        printf("  FAIL: malloc failed\n");
        return 0;
    }

    int compressed_size = 0;
    if (jocky_compress(plaintext, plaintext_size, compressed,
                       compressed_size_max, &compressed_size,
                       JOCKY_COMPRESS_BALANCED) != 0) {
        printf("  FAIL: Compression failed\n");
        free(compressed);
        return 0;
    }

    if (compressed_size <= 0 || compressed_size > compressed_size_max) {
        printf("  FAIL: Invalid compressed size: %d\n", compressed_size);
        free(compressed);
        return 0;
    }

    uint8_t decompressed[1024];
    int decompressed_size = 0;

    if (jocky_decompress(compressed, compressed_size, decompressed,
                        sizeof(decompressed), &decompressed_size) != 0) {
        printf("  FAIL: Decompression failed\n");
        free(compressed);
        return 0;
    }

    if (decompressed_size != plaintext_size ||
        memcmp(plaintext, decompressed, plaintext_size) != 0) {
        printf("  FAIL: Decompressed data mismatch\n");
        free(compressed);
        return 0;
    }

    printf("  PASS (plaintext: %d bytes, compressed: %d bytes, ratio: %.1f%%)\n",
           plaintext_size, compressed_size,
           (100.0 * compressed_size) / plaintext_size);

    free(compressed);
    return 1;
}

/* Test 2: Compression levels */
int test_compression_levels(void) {
    printf("TEST 2: Compression levels (fast vs best)\n");

    const uint8_t plaintext[] =
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. "
        "The quick brown fox jumps over the lazy dog. ";
    int plaintext_size = strlen((const char*)plaintext);

    int max_size = jocky_compress_bound(plaintext_size);

    uint8_t* fast_data = malloc(max_size);
    uint8_t* best_data = malloc(max_size);

    int fast_size = 0, best_size = 0;

    if (jocky_compress(plaintext, plaintext_size, fast_data, max_size,
                       &fast_size, JOCKY_COMPRESS_FAST) != 0) {
        printf("  FAIL: Fast compression failed\n");
        free(fast_data);
        free(best_data);
        return 0;
    }

    if (jocky_compress(plaintext, plaintext_size, best_data, max_size,
                       &best_size, JOCKY_COMPRESS_BEST) != 0) {
        printf("  FAIL: Best compression failed\n");
        free(fast_data);
        free(best_data);
        return 0;
    }

    printf("  PASS (fast: %d bytes, best: %d bytes)\n", fast_size, best_size);

    free(fast_data);
    free(best_data);
    return 1;
}

/* Test 3: Streaming compression */
int test_streaming_compress(void) {
    printf("TEST 3: Streaming compression\n");

    const uint8_t* chunks[] = {
        (const uint8_t*)"Chunk 1: ",
        (const uint8_t*)"Chunk 2: ",
        (const uint8_t*)"Chunk 3: "
    };
    int chunk_sizes[] = {9, 9, 9};

    jocky_compress_ctx_t ctx = jocky_compress_stream_create(JOCKY_COMPRESS_BALANCED);
    if (!ctx) {
        printf("  FAIL: Could not create compression context\n");
        return 0;
    }

    uint8_t compressed[1024];
    int total_compressed = 0;

    for (int i = 0; i < 3; i++) {
        int out_size = 0;
        int finish = (i == 2) ? 1 : 0;

        if (jocky_compress_stream_update(ctx, chunks[i], chunk_sizes[i],
                                        compressed + total_compressed,
                                        sizeof(compressed) - total_compressed,
                                        &out_size, finish) != 0) {
            printf("  FAIL: Stream update failed on chunk %d\n", i);
            jocky_compress_stream_destroy(ctx);
            return 0;
        }

        total_compressed += out_size;
    }

    jocky_compress_stream_destroy(ctx);

    if (total_compressed <= 0) {
        printf("  FAIL: No compressed data\n");
        return 0;
    }

    printf("  PASS (streamed %d bytes, compressed to %d bytes)\n", 27, total_compressed);
    return 1;
}

/* Test 4: Streaming decompression */
int test_streaming_decompress(void) {
    printf("TEST 4: Streaming decompression\n");

    const uint8_t plaintext[] = "Data chunk 1, Data chunk 2, Data chunk 3";
    int plaintext_size = strlen((const char*)plaintext);

    /* First compress */
    int max_size = jocky_compress_bound(plaintext_size);
    uint8_t* compressed = malloc(max_size);
    int compressed_size = 0;

    if (jocky_compress(plaintext, plaintext_size, compressed, max_size,
                       &compressed_size, JOCKY_COMPRESS_BALANCED) != 0) {
        printf("  FAIL: Compression failed\n");
        free(compressed);
        return 0;
    }

    /* Now decompress in streaming mode */
    jocky_compress_ctx_t ctx = jocky_decompress_stream_create();
    if (!ctx) {
        printf("  FAIL: Could not create decompression context\n");
        free(compressed);
        return 0;
    }

    uint8_t decompressed[1024];
    int decompressed_size = 0;

    int out_size = 0;
    if (jocky_decompress_stream_update(ctx, compressed, compressed_size,
                                      decompressed, sizeof(decompressed),
                                      &out_size, 1) != 0) {
        printf("  FAIL: Stream decompression failed\n");
        jocky_decompress_stream_destroy(ctx);
        free(compressed);
        return 0;
    }

    decompressed_size = out_size;
    jocky_decompress_stream_destroy(ctx);

    if (decompressed_size != plaintext_size ||
        memcmp(plaintext, decompressed, plaintext_size) != 0) {
        printf("  FAIL: Decompressed data mismatch\n");
        free(compressed);
        return 0;
    }

    printf("  PASS\n");
    free(compressed);
    return 1;
}

/* Test 5: Large data */
int test_large_data(void) {
    printf("TEST 5: Large data compression\n");

    int large_size = 100000;
    uint8_t* large_data = malloc(large_size);
    if (!large_data) {
        printf("  FAIL: malloc failed\n");
        return 0;
    }

    /* Fill with compressible data */
    for (int i = 0; i < large_size; i++) {
        large_data[i] = 'A' + (i % 26);
    }

    int max_size = jocky_compress_bound(large_size);
    uint8_t* compressed = malloc(max_size);
    if (!compressed) {
        printf("  FAIL: malloc failed\n");
        free(large_data);
        return 0;
    }

    int compressed_size = 0;
    if (jocky_compress(large_data, large_size, compressed, max_size,
                       &compressed_size, JOCKY_COMPRESS_BEST) != 0) {
        printf("  FAIL: Compression failed\n");
        free(large_data);
        free(compressed);
        return 0;
    }

    uint8_t* decompressed = malloc(large_size);
    if (!decompressed) {
        printf("  FAIL: malloc failed\n");
        free(large_data);
        free(compressed);
        return 0;
    }

    int decompressed_size = 0;
    if (jocky_decompress(compressed, compressed_size, decompressed, large_size,
                        &decompressed_size) != 0) {
        printf("  FAIL: Decompression failed\n");
        free(large_data);
        free(compressed);
        free(decompressed);
        return 0;
    }

    if (decompressed_size != large_size ||
        memcmp(large_data, decompressed, large_size) != 0) {
        printf("  FAIL: Data mismatch\n");
        free(large_data);
        free(compressed);
        free(decompressed);
        return 0;
    }

    printf("  PASS (100KB -> %d bytes, ratio: %.1f%%)\n", compressed_size,
           (100.0 * compressed_size) / large_size);

    free(large_data);
    free(compressed);
    free(decompressed);
    return 1;
}

/* Test 6: Empty data */
int test_empty_data(void) {
    printf("TEST 6: Empty data handling\n");

    uint8_t empty[] = "";
    uint8_t output[64];
    int out_size = 0;

    if (jocky_compress(empty, 0, output, sizeof(output), &out_size,
                       JOCKY_COMPRESS_BALANCED) == 0) {
        printf("  FAIL: Should handle empty data\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

/* Test 7: Compression bound calculation */
int test_compress_bound(void) {
    printf("TEST 7: Compression bound calculation\n");

    int sizes[] = {10, 100, 1000, 10000};
    for (int i = 0; i < 4; i++) {
        int bound = jocky_compress_bound(sizes[i]);
        if (bound <= 0) {
            printf("  FAIL: compress_bound returned %d for size %d\n",
                   bound, sizes[i]);
            return 0;
        }
    }

    printf("  PASS\n");
    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("JOCKY Compression Tests\n");
    printf("========================================\n\n");

    int passed = 0;
    int total = 0;

    #define RUN_TEST(fn) do { \
        total++; \
        if (fn()) passed++; \
        printf("\n"); \
    } while(0)

    RUN_TEST(test_basic_compress_decompress);
    RUN_TEST(test_compression_levels);
    RUN_TEST(test_streaming_compress);
    RUN_TEST(test_streaming_decompress);
    RUN_TEST(test_large_data);
    RUN_TEST(test_empty_data);
    RUN_TEST(test_compress_bound);

    printf("========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n");

    return (passed == total) ? 0 : 1;
}
