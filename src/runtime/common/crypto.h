#ifndef JOCKY_RUNTIME_COMMON_CRYPTO_H
#define JOCKY_RUNTIME_COMMON_CRYPTO_H

#include <stdint.h>
#include <stddef.h>

/* Cryptographic operations API
 * Platform-agnostic interface for encryption/decryption operations.
 * Implementations may use platform-specific crypto libraries.
 */

/* XOR-based encryption: simple XOR cipher for obfuscation
 * Returns: number of bytes processed, or -1 on error */
int32_t crypto_xor(const void *input, size_t input_len,
                   void *output, size_t output_len,
                   const void *key, size_t key_len);

/* RC4 stream cipher: stateful cipher for data obfuscation
 * Returns: 0 on success, -1 on error */
int32_t crypto_rc4_init(void *ctx, size_t ctx_len,
                        const void *key, size_t key_len);

int32_t crypto_rc4_crypt(void *ctx, const void *input,
                         size_t input_len, void *output);

/* AES encryption: symmetric encryption with 128-bit block size
 * Returns: 0 on success, -1 on error */
int32_t crypto_aes_encrypt(const void *plaintext, size_t plaintext_len,
                           void *ciphertext, size_t *ciphertext_len,
                           const void *key, size_t key_len);

/* AES decryption: reverse of encryption
 * Returns: 0 on success, -1 on error */
int32_t crypto_aes_decrypt(const void *ciphertext, size_t ciphertext_len,
                           void *plaintext, size_t *plaintext_len,
                           const void *key, size_t key_len);

#endif /* JOCKY_RUNTIME_COMMON_CRYPTO_H */
