#include "../../common/network.h"

/* All functions return -1 or NULL (failure).
 * No dependencies, always compiles.
 */

/* SSL/TLS functions */
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

/* TCP functions */
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
