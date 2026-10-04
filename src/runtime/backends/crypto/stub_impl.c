#include "../../common/crypto.h"

/* Stub implementation for platforms with no crypto support
 * All functions return -1 (error)
 * Used as last resort fallback
 */

int32_t crypto_xor(const void *input, size_t input_len,
                   void *output, size_t output_len,
                   const void *key, size_t key_len) {
    return -1;
}

int32_t crypto_rc4_init(void *ctx, size_t ctx_len,
                        const void *key, size_t key_len) {
    return -1;
}

int32_t crypto_rc4_crypt(void *ctx, const void *input,
                         size_t input_len, void *output) {
    return -1;
}

int32_t crypto_aes_encrypt(const void *plaintext, size_t plaintext_len,
                           void *ciphertext, size_t *ciphertext_len,
                           const void *key, size_t key_len) {
    return -1;
}

int32_t crypto_aes_decrypt(const void *ciphertext, size_t ciphertext_len,
                           void *plaintext, size_t *plaintext_len,
                           const void *key, size_t key_len) {
    return -1;
}
