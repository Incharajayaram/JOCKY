#ifndef JOCKY_EXFIL_H
#define JOCKY_EXFIL_H

#include <stdint.h>
#include <stddef.h>

int jocky_exfil_local_cdn(const char *endpoint_url, const char *filename,
                           const char *auth_token, const void *data,
                           uint32_t data_size, const char *metadata);

int jocky_exfil_list_cdn_files(const char *endpoint_url, const char *auth_token,
                                char *out_response, size_t response_size);

int jocky_exfil_verify_cdn_hash(const void *data, uint32_t data_size,
                                 const char *expected_hash);

int jocky_exfil_download_from_cdn(const char *endpoint_url, const char *auth_token,
                                   void **out_data, uint32_t *out_size);

#endif /* JOCKY_EXFIL_H */
