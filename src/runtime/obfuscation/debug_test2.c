/*
 * Debug test for self-modifying code
 */

#include "mutation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    printf("Test: Self-modifying code integrity\n");
    fflush(stdout);

    uint8_t payload[] = {0x90, 0x90, 0xc3};

    printf("  Creating segment...\n");
    fflush(stdout);

    SelfModifyingSegment segment = {
        .address = (uintptr_t)payload,
        .mutation_payload = payload,
        .payload_len = sizeof(payload),
        .checksum = jocky_compute_code_checksum(payload, sizeof(payload))
    };

    printf("  Checksum: 0x%x\n", segment.checksum);
    printf("  Address: 0x%lx\n", segment.address);
    printf("  Payload len: %zu\n", segment.payload_len);
    fflush(stdout);

    printf("  Verifying integrity...\n");
    fflush(stdout);

    bool valid = jocky_verify_code_integrity(&segment);
    printf("  Result: %s\n", valid ? "Valid" : "Invalid");
    fflush(stdout);

    printf("\nTest passed!\n");
    return 0;
}
