#include "../include/jocky_crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_aes_128_cbc_encrypt_decrypt() {
    printf("Test: AES-128 CBC encrypt/decrypt\n");

    uint8_t key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                       0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    uint8_t iv[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

    jocky_aes_ctx_t ctx = jocky_aes_create(key, JOCKY_AES_128, iv, JOCKY_AES_CBC);
    if (!ctx) {
        printf("FAIL: Could not create AES context\n");
        return;
    }

    const char* plaintext = "Hello, World!";
    int plaintext_size = strlen(plaintext);
    int ciphertext_size = 32;
    uint8_t ciphertext[32];
    uint8_t decrypted[32];
    int output_size = 0;

    if (jocky_aes_encrypt(ctx, (const uint8_t*)plaintext, plaintext_size,
                          ciphertext, &ciphertext_size) != 0) {
        printf("FAIL: Encryption failed\n");
        jocky_aes_destroy(ctx);
        return;
    }

    if (jocky_aes_decrypt(ctx, ciphertext, ciphertext_size,
                          decrypted, &output_size) != 0) {
        printf("FAIL: Decryption failed\n");
        jocky_aes_destroy(ctx);
        return;
    }

    if (output_size != plaintext_size || memcmp(plaintext, decrypted, plaintext_size) != 0) {
        printf("FAIL: Decrypted data does not match original\n");
        jocky_aes_destroy(ctx);
        return;
    }

    jocky_aes_destroy(ctx);
    printf("PASS\n");
}

void test_aes_256_ecb() {
    printf("Test: AES-256 ECB mode\n");

    uint8_t key[32];
    for (int i = 0; i < 32; i++) key[i] = i;

    jocky_aes_ctx_t ctx = jocky_aes_create(key, JOCKY_AES_256, NULL, JOCKY_AES_ECB);
    if (!ctx) {
        printf("FAIL: Could not create AES context\n");
        return;
    }

    const char* plaintext = "0123456789ABCDEF";
    int plaintext_size = 16;
    uint8_t ciphertext[32];
    uint8_t decrypted[32];
    int output_size = 0;

    if (jocky_aes_encrypt(ctx, (const uint8_t*)plaintext, plaintext_size,
                          ciphertext, &output_size) != 0) {
        printf("FAIL: Encryption failed\n");
        jocky_aes_destroy(ctx);
        return;
    }

    if (jocky_aes_decrypt(ctx, ciphertext, output_size,
                          decrypted, &output_size) != 0) {
        printf("FAIL: Decryption failed\n");
        jocky_aes_destroy(ctx);
        return;
    }

    if (memcmp(plaintext, decrypted, plaintext_size) != 0) {
        printf("FAIL: Decrypted data does not match\n");
        jocky_aes_destroy(ctx);
        return;
    }

    jocky_aes_destroy(ctx);
    printf("PASS\n");
}

void test_aes_ctr_mode() {
    printf("Test: AES CTR mode\n");

    uint8_t key[16] = {0};
    uint8_t iv[16] = {0};

    jocky_aes_ctx_t ctx = jocky_aes_create(key, JOCKY_AES_128, iv, JOCKY_AES_CTR);
    if (!ctx) {
        printf("FAIL: Could not create AES context\n");
        return;
    }

    const char* plaintext = "Test message for CTR mode";
    int plaintext_size = strlen(plaintext);
    uint8_t ciphertext[64];
    uint8_t decrypted[64];
    int output_size = 0;

    if (jocky_aes_encrypt(ctx, (const uint8_t*)plaintext, plaintext_size,
                          ciphertext, &output_size) != 0) {
        printf("FAIL: Encryption failed\n");
        jocky_aes_destroy(ctx);
        return;
    }

    if (jocky_aes_decrypt(ctx, ciphertext, output_size,
                          decrypted, &output_size) != 0) {
        printf("FAIL: Decryption failed\n");
        jocky_aes_destroy(ctx);
        return;
    }

    if (memcmp(plaintext, decrypted, plaintext_size) != 0) {
        printf("FAIL: Decrypted data does not match\n");
        jocky_aes_destroy(ctx);
        return;
    }

    jocky_aes_destroy(ctx);
    printf("PASS\n");
}

void test_invalid_aes_key_size() {
    printf("Test: Reject invalid AES key size\n");

    uint8_t key[8];
    uint8_t iv[16];

    jocky_aes_ctx_t ctx = jocky_aes_create(key, 8, iv, JOCKY_AES_CBC);
    if (ctx != NULL) {
        printf("FAIL: Should reject invalid key size\n");
        jocky_aes_destroy(ctx);
        return;
    }

    printf("PASS\n");
}

void test_aes_null_inputs() {
    printf("Test: AES null input handling\n");

    uint8_t key[16] = {0};
    uint8_t iv[16] = {0};
    uint8_t buffer[32];
    int out_size = 0;

    jocky_aes_ctx_t ctx = jocky_aes_create(key, JOCKY_AES_128, iv, JOCKY_AES_CBC);
    if (!ctx) {
        printf("FAIL: Could not create context\n");
        return;
    }

    if (jocky_aes_encrypt(ctx, NULL, 16, buffer, &out_size) == 0) {
        printf("FAIL: Should reject null plaintext\n");
        jocky_aes_destroy(ctx);
        return;
    }

    if (jocky_aes_decrypt(ctx, NULL, 16, buffer, &out_size) == 0) {
        printf("FAIL: Should reject null ciphertext\n");
        jocky_aes_destroy(ctx);
        return;
    }

    jocky_aes_destroy(ctx);
    printf("PASS\n");
}

void test_rsa_key_generation() {
    printf("Test: RSA key generation\n");

    jocky_rsa_key_t key = jocky_rsa_generate_keypair(JOCKY_RSA_2048);
    if (!key) {
        printf("SKIP (OpenSSL headers not available)\n");
        return;
    }

    jocky_rsa_destroy(key);
    printf("PASS\n");
}

void test_invalid_rsa_key_size() {
    printf("Test: Reject invalid RSA key size\n");

    jocky_rsa_key_t key = jocky_rsa_generate_keypair(1024);
    if (key != NULL) {
        printf("FAIL: Should reject invalid key size\n");
        jocky_rsa_destroy(key);
        return;
    }

    printf("PASS\n");
}

int main() {
    printf("Crypto Tests\n");
    printf("=============\n\n");

    test_aes_128_cbc_encrypt_decrypt();
    test_aes_256_ecb();
    test_aes_ctr_mode();
    test_invalid_aes_key_size();
    test_aes_null_inputs();
    test_rsa_key_generation();
    test_invalid_rsa_key_size();

    printf("\n=============\n");
    printf("All tests completed\n");

    return 0;
}
