#include "../include/jocky_network.h"
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
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef int SOCKET;
#endif

typedef struct {
    SOCKET fd;
    int family;
    int type;
} socket_context_t;

jocky_socket_t jocky_socket_create(int family, int type) {
    int af, socktype;

    if (family == JOCKY_AF_INET) {
        af = AF_INET;
    } else if (family == JOCKY_AF_INET6) {
        af = AF_INET6;
    } else {
        return NULL;
    }

    if (type == JOCKY_SOCK_STREAM) {
        socktype = SOCK_STREAM;
    } else if (type == JOCKY_SOCK_DGRAM) {
        socktype = SOCK_DGRAM;
    } else {
        return NULL;
    }

    socket_context_t* ctx = malloc(sizeof(socket_context_t));
    if (!ctx) return NULL;

    ctx->fd = socket(af, socktype, 0);
    if (ctx->fd == INVALID_SOCKET) {
        free(ctx);
        return NULL;
    }

    ctx->family = family;
    ctx->type = type;

    return (jocky_socket_t)ctx;
}

int jocky_socket_close(jocky_socket_t sock) {
    if (!sock) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;

#ifdef _WIN32
    int result = closesocket(ctx->fd);
#else
    int result = close(ctx->fd);
#endif

    free(ctx);
    return result == 0 ? 0 : -1;
}

int jocky_socket_connect(jocky_socket_t sock, const char* host, int port) {
    if (!sock || !host || port < 0 || port > 65535) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;

    struct addrinfo hints, *result = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = (ctx->family == JOCKY_AF_INET) ? AF_INET : AF_INET6;
    hints.ai_socktype = (ctx->type == JOCKY_SOCK_STREAM) ? SOCK_STREAM : SOCK_DGRAM;

    char port_str[16];
    snprintf(port_str, sizeof(port_str), "%d", port);

    if (getaddrinfo(host, port_str, &hints, &result) != 0) {
        return -1;
    }

    int ret = -1;
    for (struct addrinfo* p = result; p != NULL; p = p->ai_next) {
        if (connect(ctx->fd, p->ai_addr, (socklen_t)p->ai_addrlen) == 0) {
            ret = 0;
            break;
        }
    }

    freeaddrinfo(result);
    return ret;
}

int jocky_socket_bind(jocky_socket_t sock, const char* host, int port) {
    if (!sock || port < 0 || port > 65535) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;

    struct sockaddr_storage addr;
    memset(&addr, 0, sizeof(addr));

    if (ctx->family == JOCKY_AF_INET) {
        struct sockaddr_in* addr4 = (struct sockaddr_in*)&addr;
        addr4->sin_family = AF_INET;
        addr4->sin_port = htons(port);
        if (host) {
            if (inet_pton(AF_INET, host, &addr4->sin_addr) <= 0) return -1;
        } else {
            addr4->sin_addr.s_addr = htonl(INADDR_ANY);
        }
        return bind(ctx->fd, (struct sockaddr*)addr4, sizeof(*addr4)) == 0 ? 0 : -1;
    } else {
        struct sockaddr_in6* addr6 = (struct sockaddr_in6*)&addr;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = htons(port);
        if (host) {
            if (inet_pton(AF_INET6, host, &addr6->sin6_addr) <= 0) return -1;
        } else {
            addr6->sin6_addr = in6addr_any;
        }
        return bind(ctx->fd, (struct sockaddr*)addr6, sizeof(*addr6)) == 0 ? 0 : -1;
    }
}

int jocky_socket_listen(jocky_socket_t sock, int backlog) {
    if (!sock || backlog <= 0) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;
    return listen(ctx->fd, backlog) == 0 ? 0 : -1;
}

jocky_socket_t jocky_socket_accept(jocky_socket_t sock) {
    if (!sock) return NULL;

    socket_context_t* ctx = (socket_context_t*)sock;

    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);

    SOCKET client_fd = accept(ctx->fd, (struct sockaddr*)&addr, &addr_len);
    if (client_fd == INVALID_SOCKET) {
        return NULL;
    }

    socket_context_t* client_ctx = malloc(sizeof(socket_context_t));
    if (!client_ctx) {
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
        return NULL;
    }

    client_ctx->fd = client_fd;
    client_ctx->family = ctx->family;
    client_ctx->type = ctx->type;

    return (jocky_socket_t)client_ctx;
}

int jocky_socket_send(jocky_socket_t sock, const uint8_t* data, int size) {
    if (!sock || !data || size <= 0) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;
    int sent = send(ctx->fd, (const char*)data, size, 0);

    return sent >= 0 ? sent : -1;
}

int jocky_socket_recv(jocky_socket_t sock, uint8_t* buffer, int buffer_size) {
    if (!sock || !buffer || buffer_size <= 0) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;
    int received = recv(ctx->fd, (char*)buffer, buffer_size, 0);

    return received >= 0 ? received : -1;
}

int jocky_socket_sendto(jocky_socket_t sock, const uint8_t* data, int size,
                        const char* host, int port) {
    if (!sock || !data || size <= 0 || !host || port < 0 || port > 65535) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;

    struct addrinfo hints, *result = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = (ctx->family == JOCKY_AF_INET) ? AF_INET : AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;

    char port_str[16];
    snprintf(port_str, sizeof(port_str), "%d", port);

    if (getaddrinfo(host, port_str, &hints, &result) != 0) {
        return -1;
    }

    int sent = -1;
    if (result != NULL) {
        sent = sendto(ctx->fd, (const char*)data, size, 0,
                     result->ai_addr, (socklen_t)result->ai_addrlen);
    }

    freeaddrinfo(result);
    return sent >= 0 ? sent : -1;
}

int jocky_socket_recvfrom(jocky_socket_t sock, uint8_t* buffer, int buffer_size,
                          char* host_buffer, int host_buffer_size, int* out_port) {
    if (!sock || !buffer || buffer_size <= 0) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;

    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);

    int received = recvfrom(ctx->fd, (char*)buffer, buffer_size, 0,
                           (struct sockaddr*)&addr, &addr_len);

    if (received < 0) return -1;

    if (host_buffer && host_buffer_size > 0) {
        if (addr.ss_family == AF_INET) {
            struct sockaddr_in* addr4 = (struct sockaddr_in*)&addr;
            inet_ntop(AF_INET, &addr4->sin_addr, host_buffer, host_buffer_size);
            if (out_port) *out_port = ntohs(addr4->sin_port);
        } else if (addr.ss_family == AF_INET6) {
            struct sockaddr_in6* addr6 = (struct sockaddr_in6*)&addr;
            inet_ntop(AF_INET6, &addr6->sin6_addr, host_buffer, host_buffer_size);
            if (out_port) *out_port = ntohs(addr6->sin6_port);
        }
    }

    return received;
}

int jocky_socket_setopt(jocky_socket_t sock, int option, int value) {
    if (!sock) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;
    int opt;

    switch (option) {
        case JOCKY_SO_REUSEADDR:
            opt = SO_REUSEADDR;
            break;
        case JOCKY_SO_KEEPALIVE:
            opt = SO_KEEPALIVE;
            break;
        case JOCKY_SO_SNDBUF:
            opt = SO_SNDBUF;
            break;
        case JOCKY_SO_RCVBUF:
            opt = SO_RCVBUF;
            break;
        default:
            return -1;
    }

    return setsockopt(ctx->fd, SOL_SOCKET, opt, (const char*)&value,
                     sizeof(value)) == 0 ? 0 : -1;
}

int jocky_socket_getopt(jocky_socket_t sock, int option) {
    if (!sock) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;
    int opt;
    int value = 0;
    socklen_t value_len = sizeof(value);

    switch (option) {
        case JOCKY_SO_REUSEADDR:
            opt = SO_REUSEADDR;
            break;
        case JOCKY_SO_KEEPALIVE:
            opt = SO_KEEPALIVE;
            break;
        case JOCKY_SO_SNDBUF:
            opt = SO_SNDBUF;
            break;
        case JOCKY_SO_RCVBUF:
            opt = SO_RCVBUF;
            break;
        default:
            return -1;
    }

    if (getsockopt(ctx->fd, SOL_SOCKET, opt, (char*)&value, &value_len) != 0) {
        return -1;
    }

    return value;
}

int jocky_socket_get_error(jocky_socket_t sock) {
    if (!sock) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;
    int error = 0;
    socklen_t error_len = sizeof(error);

    if (getsockopt(ctx->fd, SOL_SOCKET, SO_ERROR, (char*)&error, &error_len) != 0) {
        return -1;
    }

    return error;
}

int jocky_socket_getpeername(jocky_socket_t sock, char* host_buffer,
                              int host_buffer_size, int* out_port) {
    if (!sock || !host_buffer || host_buffer_size <= 0) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;

    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);

    if (getpeername(ctx->fd, (struct sockaddr*)&addr, &addr_len) != 0) {
        return -1;
    }

    if (addr.ss_family == AF_INET) {
        struct sockaddr_in* addr4 = (struct sockaddr_in*)&addr;
        inet_ntop(AF_INET, &addr4->sin_addr, host_buffer, host_buffer_size);
        if (out_port) *out_port = ntohs(addr4->sin_port);
    } else if (addr.ss_family == AF_INET6) {
        struct sockaddr_in6* addr6 = (struct sockaddr_in6*)&addr;
        inet_ntop(AF_INET6, &addr6->sin6_addr, host_buffer, host_buffer_size);
        if (out_port) *out_port = ntohs(addr6->sin6_port);
    }

    return 0;
}

int jocky_socket_getsockname(jocky_socket_t sock, char* host_buffer,
                              int host_buffer_size, int* out_port) {
    if (!sock || !host_buffer || host_buffer_size <= 0) return -1;

    socket_context_t* ctx = (socket_context_t*)sock;

    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);

    if (getsockname(ctx->fd, (struct sockaddr*)&addr, &addr_len) != 0) {
        return -1;
    }

    if (addr.ss_family == AF_INET) {
        struct sockaddr_in* addr4 = (struct sockaddr_in*)&addr;
        inet_ntop(AF_INET, &addr4->sin_addr, host_buffer, host_buffer_size);
        if (out_port) *out_port = ntohs(addr4->sin_port);
    } else if (addr.ss_family == AF_INET6) {
        struct sockaddr_in6* addr6 = (struct sockaddr_in6*)&addr;
        inet_ntop(AF_INET6, &addr6->sin6_addr, host_buffer, host_buffer_size);
        if (out_port) *out_port = ntohs(addr6->sin6_port);
    }

    return 0;
}

int jocky_socket_resolve_host(const char* hostname, char* output_buffer,
                               int output_buffer_size) {
    if (!hostname || !output_buffer || output_buffer_size <= 0) return -1;

    struct addrinfo hints, *result = NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(hostname, NULL, &hints, &result) != 0) {
        return -1;
    }

    int ret = -1;
    if (result != NULL) {
        if (result->ai_family == AF_INET) {
            struct sockaddr_in* addr4 = (struct sockaddr_in*)result->ai_addr;
            inet_ntop(AF_INET, &addr4->sin_addr, output_buffer, output_buffer_size);
            ret = 0;
        } else if (result->ai_family == AF_INET6) {
            struct sockaddr_in6* addr6 = (struct sockaddr_in6*)result->ai_addr;
            inet_ntop(AF_INET6, &addr6->sin6_addr, output_buffer, output_buffer_size);
            ret = 0;
        }
    }

    freeaddrinfo(result);
    return ret;
}

int jocky_socket_aton(const char* addr_str, int family, uint8_t* output,
                      int output_size) {
    if (!addr_str || !output) return -1;

    int af = (family == JOCKY_AF_INET) ? AF_INET : AF_INET6;
    int required_size = (family == JOCKY_AF_INET) ? 4 : 16;

    if (output_size < required_size) return -1;

    return inet_pton(af, addr_str, output) == 1 ? 0 : -1;
}

int jocky_socket_init(void) {
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0 ? 0 : -1;
#else
    return 0;
#endif
}

int jocky_socket_cleanup(void) {
#ifdef _WIN32
    return WSACleanup() == 0 ? 0 : -1;
#else
    return 0;
#endif
}
