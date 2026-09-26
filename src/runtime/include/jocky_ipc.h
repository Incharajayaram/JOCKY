#ifndef JOCKY_IPC_H
#define JOCKY_IPC_H

#include <stddef.h>

/* Pipe flags */
#define JOCKY_PIPE_CLOEXEC   0x80000

/* Socket address families */
#define JOCKY_AF_UNIX        1
#define JOCKY_AF_INET        2
#define JOCKY_AF_INET6       10

/* Socket types */
#define JOCKY_SOCK_STREAM    1
#define JOCKY_SOCK_DGRAM     2

/* ========== PIPES ========== */

/**
 * Create a pipe.
 * fds: Output array [read_fd, write_fd]
 * Returns 0 on success, -1 on error.
 */
int jocky_pipe(long fds[2]);

/**
 * Create a pipe with flags.
 * fds: Output array [read_fd, write_fd]
 * flags: JOCKY_PIPE_CLOEXEC, etc.
 * Returns 0 on success, -1 on error.
 */
int jocky_pipe2(long fds[2], int flags);

/* ========== SOCKETS ========== */

/**
 * Create a socket.
 * domain: AF_UNIX, AF_INET, AF_INET6
 * type: SOCK_STREAM, SOCK_DGRAM
 * protocol: 0 for default
 * Returns socket file descriptor, -1 on error.
 */
long jocky_socket(int domain, int type, int protocol);

/**
 * Bind socket to address.
 * Returns 0 on success, -1 on error.
 */
int jocky_bind(long sockfd, const void* addr, size_t addrlen);

/**
 * Listen on socket.
 * backlog: Max pending connections
 * Returns 0 on success, -1 on error.
 */
int jocky_listen(long sockfd, int backlog);

/**
 * Accept connection on socket.
 * addr: Output peer address (optional)
 * addrlen: Input/output address length
 * Returns connected socket fd, -1 on error.
 */
long jocky_accept(long sockfd, void* addr, size_t* addrlen);

/**
 * Connect socket to address.
 * Returns 0 on success, -1 on error.
 */
int jocky_connect(long sockfd, const void* addr, size_t addrlen);

/**
 * Send data on socket.
 * Returns bytes sent, -1 on error.
 */
long jocky_send(long sockfd, const void* data, size_t size);

/**
 * Receive data from socket.
 * Returns bytes received, 0 on EOF, -1 on error.
 */
long jocky_recv(long sockfd, void* buffer, size_t size);

/* ========== SHARED MEMORY & SEMAPHORES ========== */

/**
 * Create/open shared memory segment.
 * Returns shmid, -1 on error.
 */
long jocky_shmget(long key, size_t size, int flags);

/**
 * Attach shared memory segment.
 * Returns pointer to segment, NULL on error.
 */
void* jocky_shmat(long shmid, const void* shmaddr, int flags);

/**
 * Detach shared memory segment.
 * Returns 0 on success, -1 on error.
 */
int jocky_shmdt(const void* shmaddr);

#endif // JOCKY_IPC_H
