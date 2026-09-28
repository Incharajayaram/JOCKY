/*
 * JOCKY CDN Exfiltration API
 * Local (Caddy) and Cloud (AWS) endpoints
 * Authorized: Red Hat + IIT Bombay Cyber Security Team
 */

#ifndef JOCKY_EXFIL_CDN_H
#define JOCKY_EXFIL_CDN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CDN configuration */
typedef struct {
    const char* endpoint_url;       /* https://research.internal:8443/upload */
    const char* auth_token;         /* Bearer token */
    const char* ca_cert_path;       /* Path to CA cert (optional) */
    uint32_t max_chunk_size;        /* Max upload size */
    uint32_t timeout_ms;            /* Request timeout */
} JOCKY_CDN_CONFIG;

/* Upload result */
typedef struct {
    int status_code;
    char filename[256];
    char path[512];
    uint32_t size;
    char hash[65];  /* SHA256 */
} JOCKY_CDN_UPLOAD_RESULT;

/* Initialize CDN configuration */
int jocky_cdn_init(const JOCKY_CDN_CONFIG* config);

/* Upload data chunk to CDN */
int jocky_exfil_local_cdn(
    const char* endpoint_url,
    const char* filename,
    const char* auth_token,
    const void* data,
    uint32_t data_size,
    const char* metadata);

/* List files on CDN */
int jocky_exfil_list_cdn_files(
    const char* endpoint_url,
    const char* auth_token,
    char* out_response,
    size_t response_size);

/* Verify uploaded data using SHA256 hash */
int jocky_exfil_verify_cdn_hash(
    const void* data,
    uint32_t data_size,
    const char* expected_hash);

/* Download data from CDN */
int jocky_exfil_download_from_cdn(
    const char* endpoint_url,
    const char* auth_token,
    void** out_data,
    uint32_t* out_size);

/* AWS S3 upload (deferred implementation) */
int jocky_exfil_aws_s3_upload(
    const char* bucket,
    const char* key,
    const void* data,
    uint32_t data_size,
    const char* region);

/* AWS CloudFront signed URL generation (deferred) */
int jocky_exfil_aws_cloudfront_url(
    const char* distribution_id,
    const char* key,
    uint32_t expires_seconds,
    char* out_url,
    size_t url_max_len);

#ifdef __cplusplus
}
#endif

#endif
