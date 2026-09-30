#include <string.h>
#include <stdlib.h>
#include <ctype.h>

static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

string jocky_data_base64_encode(i8* data, i32 size) {
    if (!data || size <= 0) return "";

    int encoded_size = ((size + 2) / 3) * 4 + 1;
    char* encoded = (char*)malloc(encoded_size);
    if (!encoded) return "";

    int pos = 0;
    int i = 0;

    for (i = 0; i < size - 2; i += 3) {
        unsigned int b = ((unsigned char)data[i] << 16) |
                        ((unsigned char)data[i+1] << 8) |
                        (unsigned char)data[i+2];

        encoded[pos++] = base64_chars[(b >> 18) & 0x3F];
        encoded[pos++] = base64_chars[(b >> 12) & 0x3F];
        encoded[pos++] = base64_chars[(b >> 6) & 0x3F];
        encoded[pos++] = base64_chars[b & 0x3F];
    }

    if (size % 3 == 1) {
        unsigned int b = (unsigned char)data[i] << 16;
        encoded[pos++] = base64_chars[(b >> 18) & 0x3F];
        encoded[pos++] = base64_chars[(b >> 12) & 0x3F];
        encoded[pos++] = '=';
        encoded[pos++] = '=';
    } else if (size % 3 == 2) {
        unsigned int b = ((unsigned char)data[i] << 16) | ((unsigned char)data[i+1] << 8);
        encoded[pos++] = base64_chars[(b >> 18) & 0x3F];
        encoded[pos++] = base64_chars[(b >> 12) & 0x3F];
        encoded[pos++] = base64_chars[(b >> 6) & 0x3F];
        encoded[pos++] = '=';
    }

    encoded[pos] = '\0';
    return (string)encoded;
}

i8* jocky_data_base64_decode(string encoded) {
    if (!encoded || strlen(encoded) == 0) return NULL;

    int encoded_len = strlen(encoded);
    int decoded_size = (encoded_len * 3) / 4;
    i8* decoded = (i8*)malloc(decoded_size + 1);

    if (!decoded) return NULL;

    int pos = 0;
    int i = 0;

    for (i = 0; i < encoded_len - 3; i += 4) {
        int b = 0;

        for (int j = 0; j < 4; j++) {
            char c = encoded[i + j];
            int val = 0;

            if (c >= 'A' && c <= 'Z') val = c - 'A';
            else if (c >= 'a' && c <= 'z') val = c - 'a' + 26;
            else if (c >= '0' && c <= '9') val = c - '0' + 52;
            else if (c == '+') val = 62;
            else if (c == '/') val = 63;
            else if (c == '=') val = 0;
            else continue;

            b = (b << 6) | val;
        }

        decoded[pos++] = (b >> 16) & 0xFF;
        if (encoded[i+2] != '=') decoded[pos++] = (b >> 8) & 0xFF;
        if (encoded[i+3] != '=') decoded[pos++] = b & 0xFF;
    }

    decoded[pos] = '\0';
    return decoded;
}

string jocky_data_hex_encode(i8* data, i32 size) {
    if (!data || size <= 0) return "";

    char* hex = (char*)malloc(size * 2 + 1);
    if (!hex) return "";

    for (int i = 0; i < size; i++) {
        sprintf(&hex[i*2], "%02X", (unsigned char)data[i]);
    }
    hex[size * 2] = '\0';

    return (string)hex;
}

i8* jocky_data_hex_decode(string hex) {
    if (!hex || strlen(hex) == 0) return NULL;

    int hex_len = strlen(hex);
    if (hex_len % 2 != 0) return NULL;

    i8* decoded = (i8*)malloc(hex_len / 2);
    if (!decoded) return NULL;

    for (int i = 0; i < hex_len; i += 2) {
        char byte[3];
        byte[0] = hex[i];
        byte[1] = hex[i+1];
        byte[2] = '\0';

        decoded[i/2] = (char)strtol(byte, NULL, 16);
    }

    return decoded;
}
