#include "../../common/crypto.h"
#include <string.h>
#include <stdlib.h>

#ifdef HAVE_OPENSSL

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rc4.h>

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
    RC4_KEY rc4_key;
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
    RC4_set_key(&rc4_ctx->rc4_key, (int)key_len, (const unsigned char *)key);

    return 0;
}

int32_t crypto_rc4_crypt(void *ctx, const void *input,
                         size_t input_len, void *output) {
    if (!ctx || !input || !output) {
        return -1;
    }

    rc4_context_t *rc4_ctx = (rc4_context_t *)ctx;
    RC4(&rc4_ctx->rc4_key, (int)input_len,
        (const unsigned char *)input, (unsigned char *)output);

    return 0;
}

int32_t crypto_aes_encrypt(const void *plaintext, size_t plaintext_len,
                           void *ciphertext, size_t *ciphertext_len,
                           const void *key, size_t key_len) {
    if (!plaintext || !ciphertext || !ciphertext_len || !key) {
        return -1;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return -1;
    }

    int cipher_id;
    switch (key_len) {
        case 16: cipher_id = EVP_aes_128_ecb(); break;
        case 24: cipher_id = EVP_aes_192_ecb(); break;
        case 32: cipher_id = EVP_aes_256_ecb(); break;
        default:
            EVP_CIPHER_CTX_free(ctx);
            return -1;
    }

    const EVP_CIPHER *cipher = (key_len == 16) ? EVP_aes_128_ecb() :
                               (key_len == 24) ? EVP_aes_192_ecb() :
                               EVP_aes_256_ecb();

    if (!EVP_EncryptInit_ex(ctx, cipher, NULL, (const unsigned char *)key, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    int len = 0;
    int ciphertext_len_int = 0;

    if (!EVP_EncryptUpdate(ctx, (unsigned char *)ciphertext, &len,
                          (const unsigned char *)plaintext, (int)plaintext_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    ciphertext_len_int += len;

    if (!EVP_EncryptFinal_ex(ctx, (unsigned char *)ciphertext + ciphertext_len_int, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    ciphertext_len_int += len;

    if ((size_t)ciphertext_len_int > *ciphertext_len) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    *ciphertext_len = ciphertext_len_int;
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

int32_t crypto_aes_decrypt(const void *ciphertext, size_t ciphertext_len,
                           void *plaintext, size_t *plaintext_len,
                           const void *key, size_t key_len) {
    if (!ciphertext || !plaintext || !plaintext_len || !key) {
        return -1;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        return -1;
    }

    const EVP_CIPHER *cipher = (key_len == 16) ? EVP_aes_128_ecb() :
                               (key_len == 24) ? EVP_aes_192_ecb() :
                               EVP_aes_256_ecb();

    if (!cipher) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    if (!EVP_DecryptInit_ex(ctx, cipher, NULL, (const unsigned char *)key, NULL)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    int len = 0;
    int plaintext_len_int = 0;

    if (!EVP_DecryptUpdate(ctx, (unsigned char *)plaintext, &len,
                          (const unsigned char *)ciphertext, (int)ciphertext_len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len_int += len;

    if (!EVP_DecryptFinal_ex(ctx, (unsigned char *)plaintext + plaintext_len_int, &len)) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    plaintext_len_int += len;

    if ((size_t)plaintext_len_int > *plaintext_len) {
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }

    *plaintext_len = plaintext_len_int;
    EVP_CIPHER_CTX_free(ctx);
    return 0;
}

#else

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

#endif
