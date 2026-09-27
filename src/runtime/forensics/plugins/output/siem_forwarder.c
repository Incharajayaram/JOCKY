#include "forensic_types.h"
/**
 * SIEM Forwarder Output Plugin
 * 
 * Forwards analysis results to SIEM via HTTP (Splunk HEC, Elastic, etc.)
 * Uses raw sockets for HTTP POST - no external dependencies.
 */

#include "forensic_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

/* ============================================================================
 * HTTP Helper Functions
 * ============================================================================ */

static int http_post(const char* url, const char* payload, int timeout_sec) {
    // Parse URL: http(s)://host:port/path
    char proto[16], host[256], path[512];
    int port = 80;
    
    if (sscanf(url, "%15[^:]://%255[^/]/%511s", proto, host, path) != 3) {
        if (sscanf(url, "%15[^:]://%255[^/]", proto, host) != 2) {
            fprintf(stderr, "[siem_forwarder] Invalid URL: %s\n", url);
            return -1;
        }
        strcpy(path, "/");
    } else {
        // Prepend / to path
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "/%s", path);
        strcpy(path, full_path);
    }
    
    // Check for port in host
    char* colon = strchr(host, ':');
    if (colon) {
        *colon = '\0';
        port = atoi(colon + 1);
    }
    
    if (strcasecmp(proto, "https") == 0) {
        port = 443;
        fprintf(stderr, "[siem_forwarder] HTTPS not supported, use HTTP or add TLS library\n");
        return -1;
    }
    
    // Resolve host
    struct hostent* he = gethostbyname(host);
    if (!he) {
        fprintf(stderr, "[siem_forwarder] DNS resolution failed for %s: %s\n", host, hstrerror(h_errno));
        return -1;
    }
    
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "[siem_forwarder] Socket creation failed: %s\n", strerror(errno));
        return -1;
    }
    
    // Set timeout
    struct timeval tv = {timeout_sec, 0};
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    
    // Connect
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    memcpy(&addr.sin_addr, he->h_addr_list[0], he->h_length);
    
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "[siem_forwarder] Connect failed to %s:%d: %s\n", host, port, strerror(errno));
        close(sockfd);
        return -1;
    }
    
    // Build HTTP request
    char request[4096];
    int payload_len = strlen(payload);
    snprintf(request, sizeof(request),
        "POST %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        path, host, payload_len, payload);
    
    // Send request
    if (send(sockfd, request, strlen(request), 0) < 0) {
        fprintf(stderr, "[siem_forwarder] Send failed: %s\n", strerror(errno));
        close(sockfd);
        return -1;
    }
    
    // Read response (just to verify connection worked)
    char response[1024];
    int bytes = recv(sockfd, response, sizeof(response) - 1, 0);
    if (bytes > 0) {
        response[bytes] = '\0';
        if (strstr(response, "200") || strstr(response, "201") || strstr(response, "202")) {
            printf("[siem_forwarder] Successfully forwarded to %s\n", url);
        } else {
            fprintf(stderr, "[siem_forwarder] Server response: %.200s\n", response);
        }
    }
    
    close(sockfd);
    return 0;
}

static char* build_json_payload(const forensic_timeline_t* timeline,
                                 const forensic_ioc_list_t* iocs,
                                 char* buffer, size_t buf_size) {
    snprintf(buffer, buf_size,
        "{\"event\":\"forensic_analysis\",\"timestamp\":%ld,"
        "\"ioc_count\":%zu,\"event_count\":%zu,"
        "\"source\":\"jocky_forensic\"}",
        (long)time(NULL), iocs ? iocs->count : 0, timeline ? timeline->count : 0);
    return buffer;
}

/* ============================================================================
 * SIEM Forwarder Implementation
 * ============================================================================ */

static int siem_forwarder_init(void* config) {
    (void)config;
    printf("[siem_forwarder] Initialized (HTTP socket client)\n");
    return 0;
}

static int siem_forwarder_generate(const forensic_timeline_t* timeline,
                                    const forensic_ioc_list_t* iocs,
                                    const forensic_metadata_t* provenance,
                                    void* config) {
    (void)provenance;
    
    const char* endpoint = config ? (const char*)config : "http://localhost:8088/services/collector";
    
    char payload[4096];
    build_json_payload(timeline, iocs, payload, sizeof(payload));
    
    printf("[siem_forwarder] Forwarding to %s\n", endpoint);
    printf("[siem_forwarder] Payload: %s\n", payload);
    
    int ret = http_post(endpoint, payload, 10);
    if (ret != 0) {
        fprintf(stderr, "[siem_forwarder] Forward failed, payload logged locally\n");
    }
    
    return 0;
}

static void siem_forwarder_cleanup(void) {
    printf("[siem_forwarder] Cleanup\n");
}

/* ============================================================================
 * Plugin Definition
 * ============================================================================ */

static const char* siem_capabilities[] = {
    "write_report",
    "network_access"
};

forensic_output_plugin_t siem_forwarder_plugin = {
    .name = "siem_forwarder",
    .version = "1.0.0",
    .description = "Forwards forensic results to SIEM via HTTP POST (Splunk HEC, Elastic, etc.)",
    .init = siem_forwarder_init,
    .generate = siem_forwarder_generate,
    .cleanup = siem_forwarder_cleanup,
    .required_capabilities = siem_capabilities,
    .capability_count = sizeof(siem_capabilities) / sizeof(siem_capabilities[0])
};