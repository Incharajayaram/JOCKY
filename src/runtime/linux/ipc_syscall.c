#include "../include/jocky_ipc.h"
#include "../include/jocky_syscall.h"

/* ========== PIPES ========== */

int jocky_pipe(long fds[2]) {
    if (!fds) return -1;
    long result = jocky_syscall1(SYS_pipe, (long)fds);
    return (result == 0) ? 0 : -1;
}

int jocky_pipe2(long fds[2], int flags) {
    if (!fds) return -1;
    long result = jocky_syscall2(SYS_pipe2, (long)fds, flags);
    return (result == 0) ? 0 : -1;
}

/* ========== SOCKETS ========== */

long jocky_socket(int domain, int type, int protocol) {
    return jocky_syscall3(SYS_socket, domain, type, protocol);
}

int jocky_bind(long sockfd, const void* addr, size_t addrlen) {
    if (sockfd < 0 || !addr) return -1;
    long result = jocky_syscall3(SYS_bind, sockfd, (long)addr, addrlen);
    return (result == 0) ? 0 : -1;
}

int jocky_listen(long sockfd, int backlog) {
    if (sockfd < 0) return -1;
    long result = jocky_syscall2(SYS_listen, sockfd, backlog);
    return (result == 0) ? 0 : -1;
}

long jocky_accept(long sockfd, void* addr, size_t* addrlen) {
    if (sockfd < 0) return -1;
    return jocky_syscall3(SYS_accept, sockfd, (long)addr, (long)addrlen);
}

int jocky_connect(long sockfd, const void* addr, size_t addrlen) {
    if (sockfd < 0 || !addr) return -1;
    long result = jocky_syscall3(SYS_connect, sockfd, (long)addr, addrlen);
    return (result == 0) ? 0 : -1;
}

long jocky_send(long sockfd, const void* data, size_t size) {
    if (sockfd < 0 || !data || size == 0) return -1;
    return jocky_syscall3(SYS_write, sockfd, (long)data, size);
}

long jocky_recv(long sockfd, void* buffer, size_t size) {
    if (sockfd < 0 || !buffer || size == 0) return -1;
    return jocky_syscall3(SYS_read, sockfd, (long)buffer, size);
}

/* ========== SHARED MEMORY & SEMAPHORES ========== */

long jocky_shmget(long key, size_t size, int flags) {
    /* IPC syscalls are architecture-specific, using generic ipc call */
    return -1;  /* Placeholder: would use SYS_ipc or architecture-specific call */
}

void* jocky_shmat(long shmid, const void* shmaddr, int flags) {
    /* IPC syscalls are architecture-specific */
    return NULL;  /* Placeholder */
}

int jocky_shmdt(const void* shmaddr) {
    if (!shmaddr) return -1;
    /* IPC syscalls are architecture-specific */
    return -1;  /* Placeholder */
}
