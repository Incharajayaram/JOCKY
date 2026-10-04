#ifdef HAVE_OPENSSL

#include "../../common/network.h"
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

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
    SSL* ssl;
    SSL_CTX* ctx;
} ssl_context_t;

/* Global SSL initialization (call once per process) */
static int openssl_initialized = 0;

static void openssl_init(void) {
    if (!openssl_initialized) {
        SSL_library_init();
        SSL_load_error_strings();
        OpenSSL_add_all_algorithms();
        openssl_initialized = 1;
    }
}

jocky_ssl_t jocky_ssl_connect(const char* host, int port) {
    if (!host || port <= 0 || port > 65535) return NULL;

    openssl_init();

    ssl_context_t* ctx = (ssl_context_t*)malloc(sizeof(ssl_context_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(*ctx));

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

    ctx->ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx->ctx) {
#ifdef _WIN32
        closesocket(ctx->fd);
#else
        close(ctx->fd);
#endif
        free(ctx);
        return NULL;
    }

    ctx->ssl = SSL_new(ctx->ctx);
    if (!ctx->ssl) {
        SSL_CTX_free(ctx->ctx);
#ifdef _WIN32
        closesocket(ctx->fd);
#else
        close(ctx->fd);
#endif
        free(ctx);
        return NULL;
    }

    SSL_set_fd(ctx->ssl, (int)ctx->fd);

    if (SSL_connect(ctx->ssl) <= 0) {
        SSL_free(ctx->ssl);
        SSL_CTX_free(ctx->ctx);
#ifdef _WIN32
        closesocket(ctx->fd);
#else
        close(ctx->fd);
#endif
        free(ctx);
        return NULL;
    }

    return (jocky_ssl_t)ctx;
}

int32_t jocky_ssl_send(jocky_ssl_t ssl_ctx, const void* data, size_t data_len) {
    if (!ssl_ctx || !data || data_len == 0) return -1;

    ssl_context_t* ctx = (ssl_context_t*)ssl_ctx;
    int ret = SSL_write(ctx->ssl, data, (int)data_len);

    return ret > 0 ? (int32_t)ret : -1;
}

int32_t jocky_ssl_recv(jocky_ssl_t ssl_ctx, void* buffer, size_t buffer_len) {
    if (!ssl_ctx || !buffer || buffer_len == 0) return -1;

    ssl_context_t* ctx = (ssl_context_t*)ssl_ctx;
    int ret = SSL_read(ctx->ssl, buffer, (int)buffer_len);

    if (ret == 0) return 0;
    if (ret < 0) {
        int err = SSL_get_error(ctx->ssl, ret);
        if (err == SSL_ERROR_ZERO_RETURN) return 0;
        return -1;
    }

    return (int32_t)ret;
}

int32_t jocky_ssl_close(jocky_ssl_t ssl_ctx) {
    if (!ssl_ctx) return -1;

    ssl_context_t* ctx = (ssl_context_t*)ssl_ctx;

    if (ctx->ssl) {
        SSL_shutdown(ctx->ssl);
        SSL_free(ctx->ssl);
    }

    if (ctx->ctx) {
        SSL_CTX_free(ctx->ctx);
    }

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

/* TCP functions: not implemented in OpenSSL backend, return -1 */

jocky_tcp_t jocky_tcp_connect(const char* host, int port) {
    (void)host;
    (void)port;
    return NULL;
}

int32_t jocky_tcp_send(jocky_tcp_t ctx, const void* data, size_t data_len) {
    (void)ctx;
    (void)data;
    (void)data_len;
    return -1;
}

int32_t jocky_tcp_recv(jocky_tcp_t ctx, void* buffer, size_t buffer_len) {
    (void)ctx;
    (void)buffer;
    (void)buffer_len;
    return -1;
}

int32_t jocky_tcp_close(jocky_tcp_t ctx) {
    (void)ctx;
    return -1;
}

#endif /* HAVE_OPENSSL */
