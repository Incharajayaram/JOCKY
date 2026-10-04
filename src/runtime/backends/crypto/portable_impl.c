#include "../../common/crypto.h"
#include <string.h>
#include <stdint.h>

/* Pure C implementation without external dependencies
 * Provides XOR encryption (always available)
 * RC4 and AES stubs (return -1)
 */

int32_t crypto_xor(const void *input, size_t input_len,
                   void *output, size_t output_len,
                   const void *key, size_t key_len) {
    if (!input || !output || !key || input_len == 0 || key_len == 0) {
        return -1;
    }

    if (output_len < input_len) {
        return -1;
    }

    const uint8_t *in = (const uint8_t *)input;
    uint8_t *out = (uint8_t *)output;
    const uint8_t *k = (const uint8_t *)key;

    for (size_t i = 0; i < input_len; i++) {
        out[i] = in[i] ^ k[i % key_len];
    }

    return (int32_t)input_len;
}

typedef struct {
    uint8_t s[256];
    uint8_t i;
    uint8_t j;
} rc4_context_t;

int32_t crypto_rc4_init(void *ctx, size_t ctx_len,
                        const void *key, size_t key_len) {
    if (!ctx || !key || key_len == 0) {
        return -1;
    }

    if (ctx_len < sizeof(rc4_context_t)) {
        return -1;
    }

    rc4_context_t *rc4_ctx = (rc4_context_t *)ctx;
    uint8_t j = 0;

    for (int i = 0; i < 256; i++) {
        rc4_ctx->s[i] = i;
    }

    const uint8_t *k = (const uint8_t *)key;
    for (int i = 0; i < 256; i++) {
        j = (j + rc4_ctx->s[i] + k[i % key_len]) % 256;

        uint8_t tmp = rc4_ctx->s[i];
        rc4_ctx->s[i] = rc4_ctx->s[j];
        rc4_ctx->s[j] = tmp;
    }

    rc4_ctx->i = 0;
    rc4_ctx->j = 0;

    return 0;
}

int32_t crypto_rc4_crypt(void *ctx, const void *input,
                         size_t input_len, void *output) {
    if (!ctx || !input || !output) {
        return -1;
    }

    rc4_context_t *rc4_ctx = (rc4_context_t *)ctx;
    const uint8_t *in = (const uint8_t *)input;
    uint8_t *out = (uint8_t *)output;

    for (size_t k = 0; k < input_len; k++) {
        rc4_ctx->i = (rc4_ctx->i + 1) % 256;
        rc4_ctx->j = (rc4_ctx->j + rc4_ctx->s[rc4_ctx->i]) % 256;

        uint8_t tmp = rc4_ctx->s[rc4_ctx->i];
        rc4_ctx->s[rc4_ctx->i] = rc4_ctx->s[rc4_ctx->j];
        rc4_ctx->s[rc4_ctx->j] = tmp;

        uint8_t idx = (rc4_ctx->s[rc4_ctx->i] + rc4_ctx->s[rc4_ctx->j]) % 256;
        out[k] = in[k] ^ rc4_ctx->s[idx];
    }

    return 0;
}

int32_t crypto_aes_encrypt(const void *plaintext, size_t plaintext_len,
                           void *ciphertext, size_t *ciphertext_len,
                           const void *key, size_t key_len) {
    /* Stub: AES requires external library for proper implementation */
    if (!plaintext || !ciphertext || !ciphertext_len || !key) {
        return -1;
    }
    return -1;
}

int32_t crypto_aes_decrypt(const void *ciphertext, size_t ciphertext_len,
                           void *plaintext, size_t *plaintext_len,
                           const void *key, size_t key_len) {
    /* Stub: AES requires external library for proper implementation */
    if (!ciphertext || !plaintext || !plaintext_len || !key) {
        return -1;
    }
    return -1;
}
