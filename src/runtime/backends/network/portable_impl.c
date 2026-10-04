#include "../../common/network.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef int SOCKET;
#endif

typedef struct {
    SOCKET fd;
} tcp_context_t;

/* Platform-specific socket initialization */
static int portable_initialized = 0;

static void portable_init(void) {
    if (!portable_initialized) {
#ifdef _WIN32
        WSADATA wsa_data;
        WSAStartup(MAKEWORD(2, 2), &wsa_data);
#endif
        portable_initialized = 1;
    }
}

/* ========== TCP IMPLEMENTATION ========== */

jocky_tcp_t jocky_tcp_connect(const char* host, int port) {
    if (!host || port <= 0 || port > 65535) return NULL;

    portable_init();

    tcp_context_t* ctx = (tcp_context_t*)malloc(sizeof(tcp_context_t));
    if (!ctx) return NULL;

    struct addrinfo hints, *result = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    char port_str[16];
    snprintf(port_str, sizeof(port_str), "%d", port);

    if (getaddrinfo(host, port_str, &hints, &result) != 0) {
        free(ctx);
        return NULL;
    }

    ctx->fd = INVALID_SOCKET;
    for (struct addrinfo* p = result; p != NULL; p = p->ai_next) {
        ctx->fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (ctx->fd == INVALID_SOCKET) continue;

        if (connect(ctx->fd, p->ai_addr, (socklen_t)p->ai_addrlen) == 0) {
            break;
        }

#ifdef _WIN32
        closesocket(ctx->fd);
#else
        close(ctx->fd);
#endif
        ctx->fd = INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (ctx->fd == INVALID_SOCKET) {
        free(ctx);
        return NULL;
    }

    return (jocky_tcp_t)ctx;
}

int32_t jocky_tcp_send(jocky_tcp_t tcp_ctx, const void* data, size_t data_len) {
    if (!tcp_ctx || !data || data_len == 0) return -1;

    tcp_context_t* ctx = (tcp_context_t*)tcp_ctx;
    int ret = send(ctx->fd, (const char*)data, (int)data_len, 0);

    return ret > 0 ? (int32_t)ret : -1;
}

int32_t jocky_tcp_recv(jocky_tcp_t tcp_ctx, void* buffer, size_t buffer_len) {
    if (!tcp_ctx || !buffer || buffer_len == 0) return -1;

    tcp_context_t* ctx = (tcp_context_t*)tcp_ctx;
    int ret = recv(ctx->fd, (char*)buffer, (int)buffer_len, 0);

    if (ret == 0) return 0;
    if (ret < 0) return -1;

    return (int32_t)ret;
}

int32_t jocky_tcp_close(jocky_tcp_t tcp_ctx) {
    if (!tcp_ctx) return -1;

    tcp_context_t* ctx = (tcp_context_t*)tcp_ctx;

    if (ctx->fd != INVALID_SOCKET) {
#ifdef _WIN32
        closesocket(ctx->fd);
#else
        close(ctx->fd);
#endif
    }

    free(ctx);
    return 0;
}

/* ========== SSL/TLS STUBS ========== */

jocky_ssl_t jocky_ssl_connect(const char* host, int port) {
    (void)host;
    (void)port;
    return NULL;
}

int32_t jocky_ssl_send(jocky_ssl_t ctx, const void* data, size_t data_len) {
    (void)ctx;
    (void)data;
    (void)data_len;
    return -1;
}

int32_t jocky_ssl_recv(jocky_ssl_t ctx, void* buffer, size_t buffer_len) {
    (void)ctx;
    (void)buffer;
    (void)buffer_len;
    return -1;
}

int32_t jocky_ssl_close(jocky_ssl_t ctx) {
    (void)ctx;
    return -1;
}
