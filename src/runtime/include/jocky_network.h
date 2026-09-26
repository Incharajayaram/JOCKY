#ifndef JOCKY_NETWORK_H
#define JOCKY_NETWORK_H

#include <stddef.h>
#include <stdint.h>

/* Socket address families */
#define JOCKY_AF_INET  1  /* IPv4 */
#define JOCKY_AF_INET6 2  /* IPv6 */

/* Socket types */
#define JOCKY_SOCK_STREAM 1  /* TCP */
#define JOCKY_SOCK_DGRAM  2  /* UDP */

/* Socket options */
#define JOCKY_SO_REUSEADDR 1
#define JOCKY_SO_KEEPALIVE 2
#define JOCKY_SO_TIMEOUT   3
#define JOCKY_SO_SNDBUF    4
#define JOCKY_SO_RCVBUF    5

/* Opaque socket handle */
typedef void* jocky_socket_t;

/* ========== SOCKET CREATION & DESTRUCTION ========== */

/**
 * Create a socket.
 * family: JOCKY_AF_INET or JOCKY_AF_INET6
 * type: JOCKY_SOCK_STREAM (TCP) or JOCKY_SOCK_DGRAM (UDP)
 * Returns NULL on failure.
 */
jocky_socket_t jocky_socket_create(int family, int type);

/**
 * Close and destroy a socket.
 * Returns 0 on success, -1 on failure.
 */
int jocky_socket_close(jocky_socket_t sock);

/* ========== CONNECTION OPERATIONS ========== */

/**
 * Connect to a remote host:port.
 * host: IP address or hostname (ASCII string)
 * port: Port number (1-65535)
 * Returns 0 on success, -1 on failure.
 */
int jocky_socket_connect(jocky_socket_t sock, const char* host, int port);

/**
 * Bind socket to local address and port.
 * host: IP address (NULL for INADDR_ANY)
 * port: Port number to bind to
 * Returns 0 on success, -1 on failure.
 */
int jocky_socket_bind(jocky_socket_t sock, const char* host, int port);

/**
 * Listen for incoming connections (TCP only).
 * backlog: Maximum pending connections (typically 5-128)
 * Returns 0 on success, -1 on failure.
 */
int jocky_socket_listen(jocky_socket_t sock, int backlog);

/**
 * Accept incoming connection (TCP only).
 * Returns new socket on success, NULL on failure.
 */
jocky_socket_t jocky_socket_accept(jocky_socket_t sock);

/* ========== SEND/RECEIVE DATA ========== */

/**
 * Send data on connected socket.
 * Returns number of bytes sent, -1 on failure.
 */
int jocky_socket_send(jocky_socket_t sock, const uint8_t* data, int size);

/**
 * Receive data from connected socket.
 * Returns number of bytes received (0 = connection closed, -1 = error).
 */
int jocky_socket_recv(jocky_socket_t sock, uint8_t* buffer, int buffer_size);

/**
 * Send data to specific address (UDP).
 * Returns number of bytes sent, -1 on failure.
 */
int jocky_socket_sendto(jocky_socket_t sock, const uint8_t* data, int size,
                        const char* host, int port);

/**
 * Receive data and get sender address (UDP).
 * host_buffer must be at least 256 bytes.
 * Returns number of bytes received (0 = closed, -1 = error).
 */
int jocky_socket_recvfrom(jocky_socket_t sock, uint8_t* buffer, int buffer_size,
                          char* host_buffer, int host_buffer_size, int* out_port);

/* ========== SOCKET OPTIONS & CONTROL ========== */

/**
 * Set socket option.
 * option: JOCKY_SO_* constant
 * value: option value (interpretation depends on option)
 * Returns 0 on success, -1 on failure.
 */
int jocky_socket_setopt(jocky_socket_t sock, int option, int value);

/**
 * Get socket option.
 * Returns option value on success, -1 on failure.
 */
int jocky_socket_getopt(jocky_socket_t sock, int option);

/**
 * Get socket error status and clear it.
 * Returns error code (0 = no error, non-zero = system errno).
 */
int jocky_socket_get_error(jocky_socket_t sock);

/**
 * Get peer address of connected socket.
 * host_buffer must be at least 256 bytes.
 * Returns 0 on success, -1 on failure.
 */
int jocky_socket_getpeername(jocky_socket_t sock, char* host_buffer,
                              int host_buffer_size, int* out_port);

/**
 * Get local address of socket.
 * host_buffer must be at least 256 bytes.
 * Returns 0 on success, -1 on failure.
 */
int jocky_socket_getsockname(jocky_socket_t sock, char* host_buffer,
                              int host_buffer_size, int* out_port);

/* ========== NETWORK UTILITIES ========== */

/**
 * Resolve hostname to IP address.
 * output_buffer must be at least 256 bytes.
 * Returns 0 on success (address stored in output_buffer), -1 on failure.
 */
int jocky_socket_resolve_host(const char* hostname, char* output_buffer,
                               int output_buffer_size);

/**
 * Convert IP address string to binary form.
 * family: JOCKY_AF_INET or JOCKY_AF_INET6
 * Returns 0 on success, -1 on failure.
 */
int jocky_socket_aton(const char* addr_str, int family, uint8_t* output,
                      int output_size);

/**
 * Initialize socket library (Windows-specific, no-op on Unix).
 * Call once before using any socket functions on Windows.
 */
int jocky_socket_init(void);

/**
 * Cleanup socket library (Windows-specific, no-op on Unix).
 * Call once when done using sockets.
 */
int jocky_socket_cleanup(void);

#endif // JOCKY_NETWORK_H
