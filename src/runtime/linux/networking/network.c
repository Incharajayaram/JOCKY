#include "../include/jocky_network.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

typedef struct {
    int fd;
    int family;
    int type;
} socket_impl_t;

jocky_socket_t jocky_socket_create(int family, int type) {
    int domain, socktype;

    if (family == JOCKY_AF_INET) {
        domain = AF_INET;
    } else if (family == JOCKY_AF_INET6) {
        domain = AF_INET6;
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

    int fd = socket(domain, socktype, 0);
    if (fd < 0) {
        return NULL;
    }

    socket_impl_t* sock = (socket_impl_t*)malloc(sizeof(socket_impl_t));
    if (!sock) {
        close(fd);
        return NULL;
    }

    sock->fd = fd;
    sock->family = domain;
    sock->type = socktype;

    return (jocky_socket_t)sock;
}

int jocky_socket_close(jocky_socket_t sock_handle) {
    if (!sock_handle) {
        return -1;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;
    if (close(sock->fd) < 0) {
        free(sock);
        return -1;
    }

    free(sock);
    return 0;
}

int jocky_socket_connect(jocky_socket_t sock_handle, const char* host, int port) {
    if (!sock_handle || !host || port <= 0 || port > 65535) {
        return -1;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;

    struct addrinfo hints, *res;
    char port_str[6];
    snprintf(port_str, sizeof(port_str), "%d", port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = sock->family;
    hints.ai_socktype = sock->type;

    if (getaddrinfo(host, port_str, &hints, &res) != 0) {
        return -1;
    }

    int result = -1;
    for (struct addrinfo* p = res; p; p = p->ai_next) {
        if (connect(sock->fd, p->ai_addr, p->ai_addrlen) == 0) {
            result = 0;
            break;
        }
    }

    freeaddrinfo(res);
    return result;
}

int jocky_socket_bind(jocky_socket_t sock_handle, const char* host, int port) {
    if (!sock_handle || port <= 0 || port > 65535) {
        return -1;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;

    if (sock->family == AF_INET) {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        if (host) {
            if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0) {
                return -1;
            }
        } else {
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        }

        if (bind(sock->fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            return -1;
        }
    } else if (sock->family == AF_INET6) {
        struct sockaddr_in6 addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin6_family = AF_INET6;
        addr.sin6_port = htons(port);

        if (host) {
            if (inet_pton(AF_INET6, host, &addr.sin6_addr) <= 0) {
                return -1;
            }
        } else {
            addr.sin6_addr = in6addr_any;
        }

        if (bind(sock->fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

int jocky_socket_listen(jocky_socket_t sock_handle, int backlog) {
    if (!sock_handle || backlog <= 0) {
        return -1;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;

    if (sock->type != SOCK_STREAM) {
        return -1;
    }

    if (listen(sock->fd, backlog) < 0) {
        return -1;
    }

    return 0;
}

jocky_socket_t jocky_socket_accept(jocky_socket_t sock_handle) {
    if (!sock_handle) {
        return NULL;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;

    if (sock->type != SOCK_STREAM) {
        return NULL;
    }

    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);

    int fd = accept(sock->fd, (struct sockaddr*)&addr, &addr_len);
    if (fd < 0) {
        return NULL;
    }

    socket_impl_t* new_sock = (socket_impl_t*)malloc(sizeof(socket_impl_t));
    if (!new_sock) {
        close(fd);
        return NULL;
    }

    new_sock->fd = fd;
    new_sock->family = sock->family;
    new_sock->type = sock->type;

    return (jocky_socket_t)new_sock;
}

int jocky_socket_send(jocky_socket_t sock_handle, const uint8_t* data, int size) {
    if (!sock_handle || !data || size <= 0) {
        return -1;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;

    ssize_t sent = send(sock->fd, data, size, 0);
    if (sent < 0) {
        return -1;
    }

    return (int)sent;
}

int jocky_socket_recv(jocky_socket_t sock_handle, uint8_t* buffer, int size) {
    if (!sock_handle || !buffer || size <= 0) {
        return -1;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;

    ssize_t received = recv(sock->fd, buffer, size, 0);
    if (received < 0) {
        return -1;
    }

    return (int)received;
}

int jocky_socket_sendto(jocky_socket_t sock_handle, const uint8_t* data, int size,
                        const char* host, int port) {
    if (!sock_handle || !data || size <= 0 || !host || port <= 0 || port > 65535) {
        return -1;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;

    struct addrinfo hints, *res;
    char port_str[6];
    snprintf(port_str, sizeof(port_str), "%d", port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = sock->family;
    hints.ai_socktype = sock->type;

    if (getaddrinfo(host, port_str, &hints, &res) != 0) {
        return -1;
    }

    int sent = -1;
    for (struct addrinfo* p = res; p; p = p->ai_next) {
        ssize_t result = sendto(sock->fd, data, size, 0, p->ai_addr, p->ai_addrlen);
        if (result >= 0) {
            sent = (int)result;
            break;
        }
    }

    freeaddrinfo(res);
    return sent;
}

int jocky_socket_recvfrom(jocky_socket_t sock_handle, uint8_t* buffer, int size,
                          char* host, int host_len, int* port) {
    if (!sock_handle || !buffer || size <= 0 || !host || host_len <= 0 || !port) {
        return -1;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;

    struct sockaddr_storage addr;
    socklen_t addr_len = sizeof(addr);

    ssize_t received = recvfrom(sock->fd, buffer, size, 0, (struct sockaddr*)&addr, &addr_len);
    if (received < 0) {
        return -1;
    }

    if (addr.ss_family == AF_INET) {
        struct sockaddr_in* addr_in = (struct sockaddr_in*)&addr;
        if (!inet_ntop(AF_INET, &addr_in->sin_addr, host, host_len)) {
            return -1;
        }
        *port = ntohs(addr_in->sin_port);
    } else if (addr.ss_family == AF_INET6) {
        struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)&addr;
        if (!inet_ntop(AF_INET6, &addr_in6->sin6_addr, host, host_len)) {
            return -1;
        }
        *port = ntohs(addr_in6->sin6_port);
    } else {
        return -1;
    }

    return (int)received;
}

int jocky_socket_set_opt(jocky_socket_t sock_handle, int opt, int value) {
    if (!sock_handle) {
        return -1;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;
    int optval = value;

    switch (opt) {
    case JOCKY_SO_REUSEADDR:
        if (setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
            return -1;
        }
        break;
    case JOCKY_SO_KEEPALIVE:
        if (setsockopt(sock->fd, SOL_SOCKET, SO_KEEPALIVE, &optval, sizeof(optval)) < 0) {
            return -1;
        }
        break;
    case JOCKY_SO_SNDBUF:
        if (setsockopt(sock->fd, SOL_SOCKET, SO_SNDBUF, &optval, sizeof(optval)) < 0) {
            return -1;
        }
        break;
    case JOCKY_SO_RCVBUF:
        if (setsockopt(sock->fd, SOL_SOCKET, SO_RCVBUF, &optval, sizeof(optval)) < 0) {
            return -1;
        }
        break;
    default:
        return -1;
    }

    return 0;
}

int jocky_socket_set_nonblocking(jocky_socket_t sock_handle, int nonblocking) {
    if (!sock_handle) {
        return -1;
    }

    socket_impl_t* sock = (socket_impl_t*)sock_handle;

    int flags = fcntl(sock->fd, F_GETFL, 0);
    if (flags < 0) {
        return -1;
    }

    if (nonblocking) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }

    if (fcntl(sock->fd, F_SETFL, flags) < 0) {
        return -1;
    }

    return 0;
}
