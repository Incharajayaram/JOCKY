/* JOCKY runtime initialization and crypto wrappers */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

static int runtime_initialized = 0;

__attribute__((constructor))
static void jocky_openssl_auto_init(void) {
    if (runtime_initialized) return;
    OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);
    runtime_initialized = 1;
}

/* Crypto wrappers */
void* crypto_generate_key(int size) {
    if (size <= 0 || size > 256) return NULL;

    void* key = malloc(size);
    if (!key) return NULL;

    if (!RAND_bytes((unsigned char*)key, size)) {
        free(key);
        return NULL;
    }

    return key;
}

void* crypto_aes256_encrypt(void* data, int len, void* key) {
    if (!data || !key || len <= 0) return NULL;

    AES_KEY aes_key;
    if (AES_set_encrypt_key((unsigned char*)key, 256, &aes_key) < 0) {
        return NULL;
    }

    int padded_len = ((len / 16) + 1) * 16;
    unsigned char* encrypted = malloc(padded_len);
    if (!encrypted) return NULL;

    memset(encrypted, 0, padded_len);
    memcpy(encrypted, data, len);

    for (int i = 0; i < padded_len; i += 16) {
        AES_encrypt(encrypted + i, encrypted + i, &aes_key);
    }

    return encrypted;
}

void* crypto_aes256_decrypt(void* data, int len, void* key) {
    if (!data || !key || len <= 0 || len % 16 != 0) return NULL;

    AES_KEY aes_key;
    if (AES_set_decrypt_key((unsigned char*)key, 256, &aes_key) < 0) {
        return NULL;
    }

    unsigned char* decrypted = malloc(len);
    if (!decrypted) return NULL;

    for (int i = 0; i < len; i += 16) {
        AES_decrypt((unsigned char*)data + i, decrypted + i, &aes_key);
    }

    return decrypted;
}

