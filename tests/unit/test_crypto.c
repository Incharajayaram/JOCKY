#include "../../src/runtime/include/jocky_crypto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test 1: RSA key generation */
int test_rsa_keygen(void) {
    printf("TEST 1: RSA key generation (2048-bit)\n");

    jocky_rsa_key_t key = jocky_rsa_generate_keypair(JOCKY_RSA_2048);
    if (!key) {
        printf("  FAIL: Could not generate RSA key\n");
        return 0;
    }

    jocky_rsa_destroy(key);
    printf("  PASS\n");
    return 1;
}

/* Test 2: RSA encryption/decryption */
int test_rsa_encrypt_decrypt(void) {
    printf("TEST 2: RSA encrypt/decrypt\n");

    jocky_rsa_key_t key = jocky_rsa_generate_keypair(JOCKY_RSA_2048);
    if (!key) {
        printf("  FAIL: Could not generate key\n");
        return 0;
    }

    uint8_t plaintext[] = "Hello, RSA World!";
    int plaintext_size = strlen((char*)plaintext);

    uint8_t ciphertext[256];
    int ciphertext_size = 0;

    if (jocky_rsa_encrypt(key, plaintext, plaintext_size, ciphertext, &ciphertext_size) != 0) {
        printf("  FAIL: Encryption failed\n");
        jocky_rsa_destroy(key);
        return 0;
    }

    if (ciphertext_size <= 0) {
        printf("  FAIL: Invalid ciphertext size\n");
        jocky_rsa_destroy(key);
        return 0;
    }

    uint8_t decrypted[256];
    int decrypted_size = 0;

    if (jocky_rsa_decrypt(key, ciphertext, ciphertext_size, decrypted, &decrypted_size) != 0) {
        printf("  FAIL: Decryption failed\n");
        jocky_rsa_destroy(key);
        return 0;
    }

    if (decrypted_size != plaintext_size || memcmp(plaintext, decrypted, plaintext_size) != 0) {
        printf("  FAIL: Decrypted text doesn't match\n");
        printf("    Original: %s\n", plaintext);
        printf("    Decrypted: %.*s\n", decrypted_size, decrypted);
        jocky_rsa_destroy(key);
        return 0;
    }

    jocky_rsa_destroy(key);
    printf("  PASS\n");
    return 1;
}

/* Test 3: RSA signing/verification */
int test_rsa_sign_verify(void) {
    printf("TEST 3: RSA sign/verify\n");

    jocky_rsa_key_t key = jocky_rsa_generate_keypair(JOCKY_RSA_2048);
    if (!key) {
        printf("  FAIL: Could not generate key\n");
        return 0;
    }

    uint8_t data[] = "Data to sign";
    int data_size = strlen((char*)data);

    uint8_t signature[256];
    int sig_size = 0;

    if (jocky_rsa_sign(key, data, data_size, signature, &sig_size) != 0) {
        printf("  FAIL: Signing failed\n");
        jocky_rsa_destroy(key);
        return 0;
    }

    if (sig_size <= 0) {
        printf("  FAIL: Invalid signature size\n");
        jocky_rsa_destroy(key);
        return 0;
    }

    int verify_result = jocky_rsa_verify(key, data, data_size, signature, sig_size);
    if (verify_result != 1) {
        printf("  FAIL: Signature verification failed (result: %d)\n", verify_result);
        jocky_rsa_destroy(key);
        return 0;
    }

    /* Try with wrong data (should fail) */
    uint8_t wrong_data[] = "Different data";
    verify_result = jocky_rsa_verify(key, wrong_data, strlen((char*)wrong_data), signature, sig_size);
    if (verify_result != 0) {
        printf("  FAIL: Should reject invalid signature\n");
        jocky_rsa_destroy(key);
        return 0;
    }

    jocky_rsa_destroy(key);
    printf("  PASS\n");
    return 1;
}

/* Test 4: RSA export public key */
int test_rsa_export_public(void) {
    printf("TEST 4: RSA export public key (PEM)\n");

    jocky_rsa_key_t key = jocky_rsa_generate_keypair(JOCKY_RSA_2048);
    if (!key) {
        printf("  FAIL: Could not generate key\n");
        return 0;
    }

    uint8_t pem[2048];
    int pem_size = sizeof(pem);

    if (jocky_rsa_export_public_pem(key, pem, &pem_size) != 0) {
        printf("  FAIL: Export failed\n");
        jocky_rsa_destroy(key);
        return 0;
    }

    if (pem_size <= 0 || memcmp(pem, "-----BEGIN PUBLIC KEY-----", 26) != 0) {
        printf("  FAIL: Invalid PEM format\n");
        jocky_rsa_destroy(key);
        return 0;
    }

    jocky_rsa_destroy(key);
    printf("  PASS\n");
    return 1;
}

/* Test 5: ECDH key generation */
int test_ecdh_keygen(void) {
    printf("TEST 5: ECDH key generation (P-256)\n");

    jocky_ecdh_key_t key = jocky_ecdh_generate_keypair(JOCKY_CURVE_P256);
    if (!key) {
        printf("  FAIL: Could not generate ECDH key\n");
        return 0;
    }

    jocky_ecdh_destroy(key);
    printf("  PASS\n");
    return 1;
}

/* Test 6: ECDH shared secret */
int test_ecdh_shared_secret(void) {
    printf("TEST 6: ECDH shared secret computation\n");

    /* Generate two key pairs */
    jocky_ecdh_key_t alice_key = jocky_ecdh_generate_keypair(JOCKY_CURVE_P256);
    jocky_ecdh_key_t bob_key = jocky_ecdh_generate_keypair(JOCKY_CURVE_P256);

    if (!alice_key || !bob_key) {
        printf("  FAIL: Could not generate keys\n");
        if (alice_key) jocky_ecdh_destroy(alice_key);
        if (bob_key) jocky_ecdh_destroy(bob_key);
        return 0;
    }

    /* Export public keys */
    uint8_t alice_pub[97];
    int alice_pub_size = sizeof(alice_pub);
    uint8_t bob_pub[97];
    int bob_pub_size = sizeof(bob_pub);

    if (jocky_ecdh_export_public_key(alice_key, alice_pub, &alice_pub_size) != 0 ||
        jocky_ecdh_export_public_key(bob_key, bob_pub, &bob_pub_size) != 0) {
        printf("  FAIL: Could not export public keys\n");
        jocky_ecdh_destroy(alice_key);
        jocky_ecdh_destroy(bob_key);
        return 0;
    }

    /* Compute shared secrets */
    uint8_t alice_secret[32];
    int alice_secret_size = sizeof(alice_secret);
    uint8_t bob_secret[32];
    int bob_secret_size = sizeof(bob_secret);

    if (jocky_ecdh_compute_shared_secret(alice_key, bob_pub, bob_pub_size,
                                        alice_secret, &alice_secret_size) != 0) {
        printf("  FAIL: Alice could not compute shared secret\n");
        jocky_ecdh_destroy(alice_key);
        jocky_ecdh_destroy(bob_key);
        return 0;
    }

    if (jocky_ecdh_compute_shared_secret(bob_key, alice_pub, alice_pub_size,
                                        bob_secret, &bob_secret_size) != 0) {
        printf("  FAIL: Bob could not compute shared secret\n");
        jocky_ecdh_destroy(alice_key);
        jocky_ecdh_destroy(bob_key);
        return 0;
    }

    /* Shared secrets should match */
    if (alice_secret_size != bob_secret_size ||
        memcmp(alice_secret, bob_secret, alice_secret_size) != 0) {
        printf("  FAIL: Shared secrets don't match\n");
        printf("    Alice size: %d, Bob size: %d\n", alice_secret_size, bob_secret_size);
        jocky_ecdh_destroy(alice_key);
        jocky_ecdh_destroy(bob_key);
        return 0;
    }

    jocky_ecdh_destroy(alice_key);
    jocky_ecdh_destroy(bob_key);
    printf("  PASS\n");
    return 1;
}

/* Test 7: Invalid RSA inputs */
int test_rsa_null_checks(void) {
    printf("TEST 7: RSA null input checks\n");

    if (jocky_rsa_generate_keypair(0) != NULL) {
        printf("  FAIL: Should reject invalid key size\n");
        return 0;
    }

    if (jocky_rsa_encrypt(NULL, NULL, 0, NULL, NULL) != -1) {
        printf("  FAIL: Should reject NULL key\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

/* Test 8: ECDH with different curves */
int test_ecdh_curves(void) {
    printf("TEST 8: ECDH different curves\n");

    int curves[] = { JOCKY_CURVE_P256, JOCKY_CURVE_P384, JOCKY_CURVE_P521 };
    const char* names[] = { "P-256", "P-384", "P-521" };

    for (int i = 0; i < 3; i++) {
        jocky_ecdh_key_t key = jocky_ecdh_generate_keypair(curves[i]);
        if (!key) {
            printf("  FAIL: Could not generate %s key\n", names[i]);
            return 0;
        }
        jocky_ecdh_destroy(key);
    }

    printf("  PASS\n");
    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("JOCKY Crypto Tests\n");
    printf("========================================\n\n");

    int passed = 0;
    int total = 0;

    #define RUN_TEST(fn) do { \
        total++; \
        if (fn()) passed++; \
        printf("\n"); \
    } while(0)

    RUN_TEST(test_rsa_keygen);
    RUN_TEST(test_rsa_encrypt_decrypt);
    RUN_TEST(test_rsa_sign_verify);
    RUN_TEST(test_rsa_export_public);
    RUN_TEST(test_ecdh_keygen);
    RUN_TEST(test_ecdh_shared_secret);
    RUN_TEST(test_rsa_null_checks);
    RUN_TEST(test_ecdh_curves);

    printf("========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n");

    return (passed == total) ? 0 : 1;
}
