/*
 * Debug test for polymorphic obfuscation
 */

#include "mutation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(void)
{
    printf("Test 1: Extract basic blocks\n");
    fflush(stdout);

    uint8_t code[] = {
        0x90, 0x90, 0xc3,
        0x90, 0x90, 0xe9, 0x00, 0x00, 0x00, 0x00,
        0x90, 0x90, 0xc3
    };

    int block_count = 0;
    BasicBlock *blocks = jocky_extract_basic_blocks(code, sizeof(code), &block_count);

    printf("  Block count: %d\n", block_count);
    fflush(stdout);

    if (blocks) {
        printf("  Blocks extracted:\n");
        for (int i = 0; i < block_count; i++) {
            printf("    Block %d: start=0x%lx, end=0x%lx\n", i, blocks[i].start, blocks[i].end);
        }
        free(blocks);
    }
    printf("  Done\n");
    fflush(stdout);

    printf("\nTest 2: Code checksum\n");
    fflush(stdout);

    uint8_t code1[] = {0x90, 0x90, 0x90, 0x90};
    uint32_t sum1 = jocky_compute_code_checksum(code1, sizeof(code1));
    printf("  Checksum: 0x%x\n", sum1);
    fflush(stdout);

    printf("\nAll debug tests passed!\n");
    return 0;
}
