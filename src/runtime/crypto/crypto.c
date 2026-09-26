#include "../include/jocky_crypto.h"
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/err.h>
#include <openssl/pem.h>

/* ========== AES CONTEXT ========== */

typedef struct {
    EVP_CIPHER_CTX* ctx;
    int mode;
} aes_context_t;

jocky_aes_ctx_t jocky_aes_create(const uint8_t* key, int key_size,
                                  const uint8_t* iv, int mode) {
    if (!key || (key_size != 16 && key_size != 24 && key_size != 32)) {
        return NULL;
    }

    aes_context_t* aes = malloc(sizeof(aes_context_t));
    if (!aes) return NULL;

    aes->ctx = EVP_CIPHER_CTX_new();
    if (!aes->ctx) {
        free(aes);
        return NULL;
    }

    aes->mode = mode;
    return (jocky_aes_ctx_t)aes;
}

int jocky_aes_encrypt(jocky_aes_ctx_t ctx_handle, const uint8_t* plaintext,
                      int plaintext_size, uint8_t* ciphertext,
                      int* output_size) {
    if (!ctx_handle || !plaintext || !ciphertext || !output_size) {
        return -1;
    }

    aes_context_t* ctx = (aes_context_t*)ctx_handle;
    int len = 0, ciphertext_len = 0;

    if (!EVP_EncryptFinal_ex(ctx->ctx, ciphertext + len, &len)) {
        return -1;
    }
    ciphertext_len += len;

    *output_size = ciphertext_len;
    return 0;
}

int jocky_aes_decrypt(jocky_aes_ctx_t ctx_handle, const uint8_t* ciphertext,
                      int ciphertext_size, uint8_t* plaintext,
                      int* output_size) {
    if (!ctx_handle || !ciphertext || !plaintext || !output_size) {
        return -1;
    }

    aes_context_t* ctx = (aes_context_t*)ctx_handle;
    int len = 0, plaintext_len = 0;

    if (!EVP_DecryptFinal_ex(ctx->ctx, plaintext + len, &len)) {
        return -1;
    }
    plaintext_len += len;

    *output_size = plaintext_len;
    return 0;
}

void jocky_aes_destroy(jocky_aes_ctx_t ctx_handle) {
    if (ctx_handle) {
        aes_context_t* ctx = (aes_context_t*)ctx_handle;
        if (ctx->ctx) {
            EVP_CIPHER_CTX_free(ctx->ctx);
        }
        free(ctx);
    }
}

/* ========== RSA KEY MANAGEMENT ========== */

typedef struct {
    RSA* rsa;
    EVP_PKEY* evp_key;
} rsa_key_t;

jocky_rsa_key_t jocky_rsa_generate_keypair(int key_bits) {
    RSA* rsa = NULL;
    EVP_PKEY* evp_key = NULL;
    BIGNUM* exponent = NULL;

    rsa_key_t* key = malloc(sizeof(rsa_key_t));
    if (!key) return NULL;

    rsa = RSA_new();
    if (!rsa) {
        free(key);
        return NULL;
    }

    exponent = BN_new();
    if (!exponent || !BN_set_word(exponent, RSA_F4)) {
        RSA_free(rsa);
        BN_free(exponent);
        free(key);
        return NULL;
    }

    if (!RSA_generate_key_ex(rsa, key_bits, exponent, NULL)) {
        RSA_free(rsa);
        BN_free(exponent);
        free(key);
        return NULL;
    }

    evp_key = EVP_PKEY_new();
    if (!evp_key || !EVP_PKEY_set1_RSA(evp_key, rsa)) {
        RSA_free(rsa);
        EVP_PKEY_free(evp_key);
        free(key);
        return NULL;
    }

    BN_free(exponent);

    key->rsa = rsa;
    key->evp_key = evp_key;
    return (jocky_rsa_key_t)key;
}

int jocky_rsa_encrypt(jocky_rsa_key_t key_handle, const uint8_t* plaintext,
                      int plaintext_size, uint8_t* ciphertext,
                      int* output_size) {
    if (!key_handle || !plaintext || !ciphertext || !output_size) {
        return -1;
    }

    rsa_key_t* key = (rsa_key_t*)key_handle;
    int key_len = RSA_size(key->rsa);

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
    if (!key_handle || !ciphertext || !plaintext || !output_size) {
        return -1;
    }

    rsa_key_t* key = (rsa_key_t*)key_handle;

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
    if (!key_handle || !data || !signature || !sig_size) {
        return -1;
    }

    rsa_key_t* key = (rsa_key_t*)key_handle;
    unsigned int len = 0;

    if (!RSA_sign(NID_sha256, (unsigned char*)data, data_size,
                  signature, &len, key->rsa)) {
        return -1;
    }

    *sig_size = len;
    return 0;
}

int jocky_rsa_verify(jocky_rsa_key_t key_handle, const uint8_t* data,
                     int data_size, const uint8_t* signature, int sig_size) {
    if (!key_handle || !data || !signature) {
        return -1;
    }

    rsa_key_t* key = (rsa_key_t*)key_handle;

    int result = RSA_verify(NID_sha256, (unsigned char*)data, data_size,
                           (unsigned char*)signature, sig_size, key->rsa);

    return result == 1 ? 1 : (result == 0 ? 0 : -1);
}

int jocky_rsa_export_public_pem(jocky_rsa_key_t key_handle, uint8_t* output,
                                int* output_size) {
    if (!key_handle || !output || !output_size) {
        return -1;
    }

    rsa_key_t* key = (rsa_key_t*)key_handle;
    BIO* bio = BIO_new(BIO_s_mem());
    if (!bio) return -1;

    if (!PEM_write_bio_RSA_PUBKEY(bio, key->rsa)) {
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
    if (!key_handle || !output || !output_size) {
        return -1;
    }

    rsa_key_t* key = (rsa_key_t*)key_handle;
    BIO* bio = BIO_new(BIO_s_mem());
    if (!bio) return -1;

    if (!PEM_write_bio_RSAPrivateKey(bio, key->rsa, NULL, NULL, 0, NULL, NULL)) {
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
    if (key_handle) {
        rsa_key_t* key = (rsa_key_t*)key_handle;
        if (key->evp_key) {
            EVP_PKEY_free(key->evp_key);
        }
        if (key->rsa) {
            RSA_free(key->rsa);
        }
        free(key);
    }
}

/* ========== ECDH KEY EXCHANGE ========== */

typedef struct {
    EC_KEY* ec_key;
    int curve_type;
} ecdh_key_t;

jocky_ecdh_key_t jocky_ecdh_generate_keypair(int curve) {
    int nid;

    switch (curve) {
        case JOCKY_CURVE_P256: nid = NID_X9_62_prime256v1; break;
        case JOCKY_CURVE_P384: nid = NID_secp384r1; break;
        case JOCKY_CURVE_P521: nid = NID_secp521r1; break;
        default: return NULL;
    }

    ecdh_key_t* key = malloc(sizeof(ecdh_key_t));
    if (!key) return NULL;

    key->ec_key = EC_KEY_new_by_curve_name(nid);
    if (!key->ec_key || !EC_KEY_generate_key(key->ec_key)) {
        EC_KEY_free(key->ec_key);
        free(key);
        return NULL;
    }

    key->curve_type = curve;
    return (jocky_ecdh_key_t)key;
}

int jocky_ecdh_export_public_key(jocky_ecdh_key_t key_handle, uint8_t* output,
                                 int* output_size) {
    if (!key_handle || !output || !output_size) {
        return -1;
    }

    ecdh_key_t* key = (ecdh_key_t*)key_handle;
    const EC_POINT* pub_key = EC_KEY_get0_public_key(key->ec_key);
    if (!pub_key) return -1;

    const EC_GROUP* group = EC_KEY_get0_group(key->ec_key);
    BN_CTX* ctx = BN_CTX_new();
    if (!ctx) return -1;

    size_t len = EC_POINT_point2oct(group, pub_key, POINT_CONVERSION_UNCOMPRESSED,
                                    output, *output_size, ctx);

    BN_CTX_free(ctx);

    if (len == 0) return -1;

    *output_size = len;
    return 0;
}

int jocky_ecdh_compute_shared_secret(jocky_ecdh_key_t key_handle,
                                     const uint8_t* peer_public_key,
                                     int peer_key_size,
                                     uint8_t* shared_secret,
                                     int* shared_secret_size) {
    if (!key_handle || !peer_public_key || !shared_secret || !shared_secret_size) {
        return -1;
    }

    ecdh_key_t* key = (ecdh_key_t*)key_handle;
    const EC_GROUP* group = EC_KEY_get0_group(key->ec_key);

    EC_POINT* peer_point = EC_POINT_new(group);
    if (!peer_point) return -1;

    BN_CTX* ctx = BN_CTX_new();
    if (!ctx) {
        EC_POINT_free(peer_point);
        return -1;
    }

    if (!EC_POINT_oct2point(group, peer_point, peer_public_key, peer_key_size, ctx)) {
        BN_CTX_free(ctx);
        EC_POINT_free(peer_point);
        return -1;
    }

    int field_size = EC_GROUP_get_degree(group);
    int secret_len = (field_size + 7) / 8;

    const BIGNUM* priv_key = EC_KEY_get0_private_key(key->ec_key);
    EC_POINT* result = EC_POINT_new(group);
    if (!result) {
        BN_CTX_free(ctx);
        EC_POINT_free(peer_point);
        return -1;
    }

    if (!EC_POINT_mul(group, result, NULL, peer_point, priv_key, ctx)) {
        BN_CTX_free(ctx);
        EC_POINT_free(peer_point);
        EC_POINT_free(result);
        return -1;
    }

    BIGNUM* x = BN_new();
    BIGNUM* y = BN_new();
    if (!x || !y || !EC_POINT_get_affine_coordinates(group, result, x, y, ctx)) {
        BN_free(x);
        BN_free(y);
        BN_CTX_free(ctx);
        EC_POINT_free(peer_point);
        EC_POINT_free(result);
        return -1;
    }

    if (BN_num_bytes(x) > *shared_secret_size) {
        BN_free(x);
        BN_free(y);
        BN_CTX_free(ctx);
        EC_POINT_free(peer_point);
        EC_POINT_free(result);
        return -1;
    }

    int len = BN_bn2bin(x, shared_secret);
    *shared_secret_size = len;

    BN_free(x);
    BN_free(y);
    BN_CTX_free(ctx);
    EC_POINT_free(peer_point);
    EC_POINT_free(result);

    return 0;
}

void jocky_ecdh_destroy(jocky_ecdh_key_t key_handle) {
    if (key_handle) {
        ecdh_key_t* key = (ecdh_key_t*)key_handle;
        if (key->ec_key) {
            EC_KEY_free(key->ec_key);
        }
        free(key);
    }
}
