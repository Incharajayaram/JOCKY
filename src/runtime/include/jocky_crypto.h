#ifndef JOCKY_CRYPTO_H
#define JOCKY_CRYPTO_H

#include <stddef.h>
#include <stdint.h>

/* AES encryption modes */
#define JOCKY_AES_ECB 1
#define JOCKY_AES_CBC 2
#define JOCKY_AES_CTR 3

/* AES key sizes (in bits) */
#define JOCKY_AES_128 16  /* 128-bit key = 16 bytes */
#define JOCKY_AES_192 24  /* 192-bit key = 24 bytes */
#define JOCKY_AES_256 32  /* 256-bit key = 32 bytes */

/* RSA key sizes (in bits) */
#define JOCKY_RSA_2048 2048
#define JOCKY_RSA_3072 3072
#define JOCKY_RSA_4096 4096

/* ECDH curve types */
#define JOCKY_CURVE_P256 1
#define JOCKY_CURVE_P384 2
#define JOCKY_CURVE_P521 3

/* Opaque types */
typedef void* jocky_aes_ctx_t;
typedef void* jocky_rsa_key_t;
typedef void* jocky_ecdh_key_t;

/* ========== AES ENCRYPTION ========== */

/**
 * Create AES context with key and IV.
 * mode: JOCKY_AES_ECB, JOCKY_AES_CBC, or JOCKY_AES_CTR
 * key_size: JOCKY_AES_128, JOCKY_AES_192, or JOCKY_AES_256
 * Returns NULL on failure.
 */
jocky_aes_ctx_t jocky_aes_create(const uint8_t* key, int key_size,
                                  const uint8_t* iv, int mode);

/**
 * Encrypt plaintext with AES.
 * output_size must be >= input_size (rounded up to block size).
 * Returns 0 on success, -1 on failure.
 */
int jocky_aes_encrypt(jocky_aes_ctx_t ctx, const uint8_t* plaintext,
                      int plaintext_size, uint8_t* ciphertext,
                      int* output_size);

/**
 * Decrypt ciphertext with AES.
 * output_size must be >= input_size.
 * Returns 0 on success, -1 on failure.
 */
int jocky_aes_decrypt(jocky_aes_ctx_t ctx, const uint8_t* ciphertext,
                      int ciphertext_size, uint8_t* plaintext,
                      int* output_size);

/**
 * Destroy AES context and free resources.
 */
void jocky_aes_destroy(jocky_aes_ctx_t ctx);

/* ========== RSA ENCRYPTION ========== */

/**
 * Generate RSA key pair.
 * key_bits: JOCKY_RSA_2048, JOCKY_RSA_3072, or JOCKY_RSA_4096
 * Returns NULL on failure.
 */
jocky_rsa_key_t jocky_rsa_generate_keypair(int key_bits);

/**
 * Encrypt plaintext with RSA public key.
 * output_size must be >= key_bits / 8.
 * Returns 0 on success, -1 on failure.
 */
int jocky_rsa_encrypt(jocky_rsa_key_t key, const uint8_t* plaintext,
                      int plaintext_size, uint8_t* ciphertext,
                      int* output_size);

/**
 * Decrypt ciphertext with RSA private key.
 * output_size must be >= key_bits / 8.
 * Returns 0 on success, -1 on failure.
 */
int jocky_rsa_decrypt(jocky_rsa_key_t key, const uint8_t* ciphertext,
                      int ciphertext_size, uint8_t* plaintext,
                      int* output_size);

/**
 * Sign data with RSA private key (PKCS#1 v1.5).
 * Returns 0 on success, -1 on failure.
 */
int jocky_rsa_sign(jocky_rsa_key_t key, const uint8_t* data,
                   int data_size, uint8_t* signature, int* sig_size);

/**
 * Verify signature with RSA public key.
 * Returns 1 if valid, 0 if invalid, -1 on error.
 */
int jocky_rsa_verify(jocky_rsa_key_t key, const uint8_t* data,
                     int data_size, const uint8_t* signature, int sig_size);

/**
 * Export public key to PEM format.
 * output_size should be at least 1024 bytes for 2048-bit key.
 * Returns 0 on success, -1 on failure.
 */
int jocky_rsa_export_public_pem(jocky_rsa_key_t key, uint8_t* output,
                                int* output_size);

/**
 * Export private key to PEM format (unencrypted).
 * output_size should be at least 2048 bytes for 2048-bit key.
 * Returns 0 on success, -1 on failure.
 */
int jocky_rsa_export_private_pem(jocky_rsa_key_t key, uint8_t* output,
                                 int* output_size);

/**
 * Destroy RSA key and free resources.
 */
void jocky_rsa_destroy(jocky_rsa_key_t key);

/* ========== ECDH KEY EXCHANGE ========== */

/**
 * Generate ECDH key pair on specified curve.
 * curve: JOCKY_CURVE_P256, JOCKY_CURVE_P384, or JOCKY_CURVE_P521
 * Returns NULL on failure.
 */
jocky_ecdh_key_t jocky_ecdh_generate_keypair(int curve);

/**
 * Export ECDH public key.
 * output_size should be at least 97 bytes for P256.
 * Returns 0 on success, -1 on failure.
 */
int jocky_ecdh_export_public_key(jocky_ecdh_key_t key, uint8_t* output,
                                 int* output_size);

/**
 * Compute shared secret from peer's public key.
 * shared_secret_size should be at least 32 bytes for P256.
 * Returns 0 on success, -1 on failure.
 */
int jocky_ecdh_compute_shared_secret(jocky_ecdh_key_t key,
                                     const uint8_t* peer_public_key,
                                     int peer_key_size,
                                     uint8_t* shared_secret,
                                     int* shared_secret_size);

/**
 * Destroy ECDH key and free resources.
 */
void jocky_ecdh_destroy(jocky_ecdh_key_t key);

#endif // JOCKY_CRYPTO_H
