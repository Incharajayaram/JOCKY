#include "../include/jocky_crypto.h"
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

typedef struct {
    AES_KEY key;
    uint8_t iv[16];
    int mode;
    int key_size;
} aes_ctx_impl_t;

jocky_aes_ctx_t jocky_aes_create(const uint8_t* key, int key_size,
                                  const uint8_t* iv, int mode) {
    if (!key || (key_size != JOCKY_AES_128 && key_size != JOCKY_AES_192 && key_size != JOCKY_AES_256)) {
        return NULL;
    }

    if ((mode != JOCKY_AES_ECB && mode != JOCKY_AES_CBC && mode != JOCKY_AES_CTR) ||
        (mode != JOCKY_AES_ECB && !iv)) {
        return NULL;
    }

    aes_ctx_impl_t* ctx = (aes_ctx_impl_t*)malloc(sizeof(aes_ctx_impl_t));
    if (!ctx) {
        return NULL;
    }

    if (AES_set_encrypt_key(key, key_size * 8, &ctx->key) != 0) {
        free(ctx);
        return NULL;
    }

    ctx->mode = mode;
    ctx->key_size = key_size;
    if (iv) {
        memcpy(ctx->iv, iv, 16);
    } else {
        memset(ctx->iv, 0, 16);
    }

    return (jocky_aes_ctx_t)ctx;
}

int jocky_aes_encrypt(jocky_aes_ctx_t ctx_handle, const uint8_t* plaintext,
                      int plaintext_size, uint8_t* ciphertext,
                      int* output_size) {
    if (!ctx_handle || !plaintext || plaintext_size <= 0 || !ciphertext || !output_size) {
        return -1;
    }

    aes_ctx_impl_t* ctx = (aes_ctx_impl_t*)ctx_handle;

    int block_size = 16;
    int padded_size = ((plaintext_size + block_size - 1) / block_size) * block_size;

    uint8_t* padded = (uint8_t*)malloc(padded_size);
    if (!padded) {
        return -1;
    }

    memcpy(padded, plaintext, plaintext_size);
    int padding_length = padded_size - plaintext_size;
    for (int i = 0; i < padding_length; i++) {
        padded[plaintext_size + i] = padding_length;
    }

    if (ctx->mode == JOCKY_AES_ECB) {
        for (int i = 0; i < padded_size; i += 16) {
            AES_encrypt(padded + i, ciphertext + i, &ctx->key);
        }
    } else if (ctx->mode == JOCKY_AES_CBC) {
        uint8_t iv_copy[16];
        memcpy(iv_copy, ctx->iv, 16);
        for (int i = 0; i < padded_size; i += 16) {
            for (int j = 0; j < 16; j++) {
                padded[i + j] ^= iv_copy[j];
            }
            AES_encrypt(padded + i, ciphertext + i, &ctx->key);
            memcpy(iv_copy, ciphertext + i, 16);
        }
    } else if (ctx->mode == JOCKY_AES_CTR) {
        uint8_t counter[16];
        memcpy(counter, ctx->iv, 16);
        for (int i = 0; i < padded_size; i += 16) {
            uint8_t keystream[16];
            AES_encrypt(counter, keystream, &ctx->key);
            for (int j = 0; j < 16 && (i + j) < padded_size; j++) {
                ciphertext[i + j] = padded[i + j] ^ keystream[j];
            }
            for (int j = 15; j >= 0; j--) {
                if (++counter[j] != 0) break;
            }
        }
    }

    free(padded);
    *output_size = padded_size;
    return 0;
}

int jocky_aes_decrypt(jocky_aes_ctx_t ctx_handle, const uint8_t* ciphertext,
                      int ciphertext_size, uint8_t* plaintext,
                      int* output_size) {
    if (!ctx_handle || !ciphertext || ciphertext_size <= 0 || !plaintext || !output_size) {
        return -1;
    }

    if (ciphertext_size % 16 != 0) {
        return -1;
    }

    aes_ctx_impl_t* ctx = (aes_ctx_impl_t*)ctx_handle;
    AES_KEY decrypt_key;

    if (AES_set_decrypt_key((const unsigned char*)&ctx->key, ctx->key_size * 8, &decrypt_key) != 0) {
        return -1;
    }

    if (ctx->mode == JOCKY_AES_ECB) {
        for (int i = 0; i < ciphertext_size; i += 16) {
            AES_decrypt(ciphertext + i, plaintext + i, &decrypt_key);
        }
    } else if (ctx->mode == JOCKY_AES_CBC) {
        uint8_t iv_copy[16];
        memcpy(iv_copy, ctx->iv, 16);
        for (int i = 0; i < ciphertext_size; i += 16) {
            uint8_t block[16];
            AES_decrypt(ciphertext + i, block, &decrypt_key);
            for (int j = 0; j < 16; j++) {
                plaintext[i + j] = block[j] ^ iv_copy[j];
            }
            memcpy(iv_copy, ciphertext + i, 16);
        }
    } else if (ctx->mode == JOCKY_AES_CTR) {
        uint8_t counter[16];
        memcpy(counter, ctx->iv, 16);
        for (int i = 0; i < ciphertext_size; i += 16) {
            uint8_t keystream[16];
            AES_encrypt(counter, keystream, &ctx->key);
            for (int j = 0; j < 16 && (i + j) < ciphertext_size; j++) {
                plaintext[i + j] = ciphertext[i + j] ^ keystream[j];
            }
            for (int j = 15; j >= 0; j--) {
                if (++counter[j] != 0) break;
            }
        }
    }

    int padding_length = plaintext[ciphertext_size - 1];
    if (padding_length > 16 || padding_length <= 0) {
        return -1;
    }

    for (int i = 0; i < padding_length; i++) {
        if (plaintext[ciphertext_size - 1 - i] != padding_length) {
            return -1;
        }
    }

    *output_size = ciphertext_size - padding_length;
    return 0;
}

void jocky_aes_destroy(jocky_aes_ctx_t ctx_handle) {
    if (!ctx_handle) {
        return;
    }

    aes_ctx_impl_t* ctx = (aes_ctx_impl_t*)ctx_handle;
    memset(ctx, 0, sizeof(aes_ctx_impl_t));
    free(ctx);
}

typedef struct {
    RSA* rsa;
} rsa_key_impl_t;

jocky_rsa_key_t jocky_rsa_generate_keypair(int key_bits) {
    if (key_bits != JOCKY_RSA_2048 && key_bits != JOCKY_RSA_3072 && key_bits != JOCKY_RSA_4096) {
        return NULL;
    }

    RSA* rsa = RSA_new();
    BIGNUM* e = BN_new();

    if (!rsa || !e) {
        BN_free(e);
        RSA_free(rsa);
        return NULL;
    }

    BN_set_word(e, RSA_F4);

    if (RSA_generate_key_ex(rsa, key_bits, e, NULL) != 1) {
        BN_free(e);
        RSA_free(rsa);
        return NULL;
    }

    BN_free(e);

    rsa_key_impl_t* key = (rsa_key_impl_t*)malloc(sizeof(rsa_key_impl_t));
    if (!key) {
        RSA_free(rsa);
        return NULL;
    }

    key->rsa = rsa;
    return (jocky_rsa_key_t)key;
}

int jocky_rsa_encrypt(jocky_rsa_key_t key_handle, const uint8_t* plaintext,
                      int plaintext_size, uint8_t* ciphertext,
                      int* output_size) {
    if (!key_handle || !plaintext || plaintext_size <= 0 || !ciphertext || !output_size) {
        return -1;
    }

    rsa_key_impl_t* key = (rsa_key_impl_t*)key_handle;

    int result = RSA_public_encrypt(plaintext_size, (unsigned char*)plaintext,
                                    ciphertext, key->rsa, RSA_PKCS1_PADDING);

    if (result < 0) {
        return -1;
    }

    *output_size = result;
    return 0;
}

int jocky_rsa_decrypt(jocky_rsa_key_t key_handle, const uint8_t* ciphertext,
                      int ciphertext_size, uint8_t* plaintext,
                      int* output_size) {
    if (!key_handle || !ciphertext || ciphertext_size <= 0 || !plaintext || !output_size) {
        return -1;
    }

    rsa_key_impl_t* key = (rsa_key_impl_t*)key_handle;

    int result = RSA_private_decrypt(ciphertext_size, (unsigned char*)ciphertext,
                                     plaintext, key->rsa, RSA_PKCS1_PADDING);

    if (result < 0) {
        return -1;
    }

    *output_size = result;
    return 0;
}

int jocky_rsa_sign(jocky_rsa_key_t key_handle, const uint8_t* data,
                   int data_size, uint8_t* signature, int* sig_size) {
    if (!key_handle || !data || data_size <= 0 || !signature || !sig_size) {
        return -1;
    }

    rsa_key_impl_t* key = (rsa_key_impl_t*)key_handle;
    unsigned int sig_len = 0;

    if (RSA_sign(NID_sha256, data, data_size, signature, &sig_len, key->rsa) != 1) {
        return -1;
    }

    *sig_size = sig_len;
    return 0;
}

int jocky_rsa_verify(jocky_rsa_key_t key_handle, const uint8_t* data,
                     int data_size, const uint8_t* signature, int sig_size) {
    if (!key_handle || !data || data_size <= 0 || !signature || sig_size <= 0) {
        return -1;
    }

    rsa_key_impl_t* key = (rsa_key_impl_t*)key_handle;

    int result = RSA_verify(NID_sha256, data, data_size, (unsigned char*)signature,
                           sig_size, key->rsa);

    if (result < 0) {
        return -1;
    }

    return result;
}

int jocky_rsa_export_public_pem(jocky_rsa_key_t key_handle, uint8_t* output,
                                int* output_size) {
    if (!key_handle || !output || !output_size || *output_size <= 0) {
        return -1;
    }

    rsa_key_impl_t* key = (rsa_key_impl_t*)key_handle;

    BIO* bio = BIO_new(BIO_s_mem());
    if (!bio) {
        return -1;
    }

    if (PEM_write_bio_RSAPublicKey(bio, key->rsa) != 1) {
        BIO_free(bio);
        return -1;
    }

    int len = BIO_pending(bio);
    if (len > *output_size) {
        BIO_free(bio);
        return -1;
    }

    BIO_read(bio, output, len);
    *output_size = len;
    BIO_free(bio);

    return 0;
}

int jocky_rsa_export_private_pem(jocky_rsa_key_t key_handle, uint8_t* output,
                                 int* output_size) {
    if (!key_handle || !output || !output_size || *output_size <= 0) {
        return -1;
    }

    rsa_key_impl_t* key = (rsa_key_impl_t*)key_handle;

    BIO* bio = BIO_new(BIO_s_mem());
    if (!bio) {
        return -1;
    }

    if (PEM_write_bio_RSAPrivateKey(bio, key->rsa, NULL, NULL, 0, NULL, NULL) != 1) {
        BIO_free(bio);
        return -1;
    }

    int len = BIO_pending(bio);
    if (len > *output_size) {
        BIO_free(bio);
        return -1;
    }

    BIO_read(bio, output, len);
    *output_size = len;
    BIO_free(bio);

    return 0;
}

void jocky_rsa_destroy(jocky_rsa_key_t key_handle) {
    if (!key_handle) {
        return;
    }

    rsa_key_impl_t* key = (rsa_key_impl_t*)key_handle;
    RSA_free(key->rsa);
    free(key);
}
