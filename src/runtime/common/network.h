#ifndef JOCKY_RUNTIME_COMMON_NETWORK_H
#define JOCKY_RUNTIME_COMMON_NETWORK_H

#include <stdint.h>
#include <stddef.h>

/* Network operations API
 * Platform-agnostic interface for TCP and TLS connections.
 * Implementations may use platform-specific socket and crypto libraries.
 */

/* Opaque SSL/TLS connection context */
typedef void* jocky_ssl_t;
typedef void* jocky_tcp_t;

/* ========== SSL/TLS CONNECTIONS ========== */

/* Establish TLS connection to remote host
 * host: hostname or IP address (ASCII string)
 * port: port number (1-65535)
 * Returns opaque SSL context on success, NULL on failure */
jocky_ssl_t jocky_ssl_connect(const char* host, int port);

/* Send data over TLS connection
 * Returns number of bytes sent on success, -1 on error */
int32_t jocky_ssl_send(jocky_ssl_t ctx, const void* data, size_t data_len);

/* Receive data from TLS connection
 * Returns number of bytes received on success, -1 on error, 0 on connection closed */
int32_t jocky_ssl_recv(jocky_ssl_t ctx, void* buffer, size_t buffer_len);

/* Close TLS connection and free context
 * Returns 0 on success, -1 on error */
int32_t jocky_ssl_close(jocky_ssl_t ctx);

/* ========== TCP CONNECTIONS ========== */

/* Establish plain TCP connection (no TLS)
 * host: hostname or IP address (ASCII string)
 * port: port number (1-65535)
 * Returns opaque TCP context on success, NULL on failure */
jocky_tcp_t jocky_tcp_connect(const char* host, int port);

/* Send data over TCP connection
 * Returns number of bytes sent on success, -1 on error */
int32_t jocky_tcp_send(jocky_tcp_t ctx, const void* data, size_t data_len);

/* Receive data from TCP connection
 * Returns number of bytes received on success, -1 on error, 0 on connection closed */
int32_t jocky_tcp_recv(jocky_tcp_t ctx, void* buffer, size_t buffer_len);

/* Close TCP connection and free context
 * Returns 0 on success, -1 on error */
int32_t jocky_tcp_close(jocky_tcp_t ctx);

#endif /* JOCKY_RUNTIME_COMMON_NETWORK_H */
