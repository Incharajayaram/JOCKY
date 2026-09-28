#include "../include/jocky_network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

void test_socket_create_ipv4() {
    printf("Test: Create IPv4 TCP socket\n");
    jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);
    if (!sock) {
        printf("FAIL: Could not create socket\n");
        return;
    }
    if (jocky_socket_close(sock) != 0) {
        printf("FAIL: Could not close socket\n");
        return;
    }
    printf("PASS\n");
}

void test_socket_create_ipv6() {
    printf("Test: Create IPv6 TCP socket\n");
    jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET6, JOCKY_SOCK_STREAM);
    if (!sock) {
        printf("FAIL: Could not create socket\n");
        return;
    }
    if (jocky_socket_close(sock) != 0) {
        printf("FAIL: Could not close socket\n");
        return;
    }
    printf("PASS\n");
}

void test_socket_create_udp() {
    printf("Test: Create UDP socket\n");
    jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_DGRAM);
    if (!sock) {
        printf("FAIL: Could not create socket\n");
        return;
    }
    if (jocky_socket_close(sock) != 0) {
        printf("FAIL: Could not close socket\n");
        return;
    }
    printf("PASS\n");
}

void test_invalid_family() {
    printf("Test: Reject invalid address family\n");
    jocky_socket_t sock = jocky_socket_create(99, JOCKY_SOCK_STREAM);
    if (sock != NULL) {
        printf("FAIL: Should reject invalid family\n");
        jocky_socket_close(sock);
        return;
    }
    printf("PASS\n");
}

void test_invalid_type() {
    printf("Test: Reject invalid socket type\n");
    jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET, 99);
    if (sock != NULL) {
        printf("FAIL: Should reject invalid type\n");
        jocky_socket_close(sock);
        return;
    }
    printf("PASS\n");
}

typedef struct {
    int port;
    char* message;
} server_context_t;

void* tcp_server_thread(void* arg) {
    server_context_t* ctx = (server_context_t*)arg;
    jocky_socket_t server = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);
    if (!server) {
        pthread_exit(NULL);
    }

    jocky_socket_set_opt(server, JOCKY_SO_REUSEADDR, 1);

    if (jocky_socket_bind(server, "127.0.0.1", ctx->port) != 0) {
        jocky_socket_close(server);
        pthread_exit(NULL);
    }

    if (jocky_socket_listen(server, 5) != 0) {
        jocky_socket_close(server);
        pthread_exit(NULL);
    }

    jocky_socket_t client = jocky_socket_accept(server);
    if (client) {
        jocky_socket_send(client, (const uint8_t*)ctx->message, strlen(ctx->message));
        jocky_socket_close(client);
    }

    jocky_socket_close(server);
    pthread_exit(NULL);
}

void test_tcp_connection() {
    printf("Test: TCP connection and send/receive\n");

    server_context_t ctx;
    ctx.port = 9001;
    ctx.message = "Hello from server";

    pthread_t server_tid;
    if (pthread_create(&server_tid, NULL, tcp_server_thread, &ctx) != 0) {
        printf("FAIL: Could not create server thread\n");
        return;
    }

    sleep(1);

    jocky_socket_t client = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);
    if (!client) {
        printf("FAIL: Could not create client socket\n");
        pthread_join(server_tid, NULL);
        return;
    }

    if (jocky_socket_connect(client, "127.0.0.1", 9001) != 0) {
        printf("FAIL: Could not connect\n");
        jocky_socket_close(client);
        pthread_join(server_tid, NULL);
        return;
    }

    uint8_t buffer[256];
    int received = jocky_socket_recv(client, buffer, sizeof(buffer) - 1);
    if (received <= 0) {
        printf("FAIL: Could not receive data\n");
        jocky_socket_close(client);
        pthread_join(server_tid, NULL);
        return;
    }

    buffer[received] = '\0';
    if (strcmp((const char*)buffer, ctx.message) != 0) {
        printf("FAIL: Received wrong message\n");
        jocky_socket_close(client);
        pthread_join(server_tid, NULL);
        return;
    }

    jocky_socket_close(client);
    pthread_join(server_tid, NULL);
    printf("PASS\n");
}

void test_udp_sendto_recvfrom() {
    printf("Test: UDP sendto and recvfrom\n");

    jocky_socket_t server = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_DGRAM);
    if (!server) {
        printf("FAIL: Could not create server socket\n");
        return;
    }

    if (jocky_socket_bind(server, "127.0.0.1", 9002) != 0) {
        printf("FAIL: Could not bind\n");
        jocky_socket_close(server);
        return;
    }

    jocky_socket_t client = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_DGRAM);
    if (!client) {
        printf("FAIL: Could not create client socket\n");
        jocky_socket_close(server);
        return;
    }

    const char* message = "UDP test message";
    if (jocky_socket_sendto(client, (const uint8_t*)message, strlen(message), "127.0.0.1", 9002) < 0) {
        printf("FAIL: Could not send\n");
        jocky_socket_close(client);
        jocky_socket_close(server);
        return;
    }

    uint8_t buffer[256];
    char host[256];
    int port = 0;
    int received = jocky_socket_recvfrom(server, buffer, sizeof(buffer) - 1, host, sizeof(host), &port);

    if (received <= 0) {
        printf("FAIL: Could not receive\n");
        jocky_socket_close(client);
        jocky_socket_close(server);
        return;
    }

    buffer[received] = '\0';
    if (strcmp((const char*)buffer, message) != 0) {
        printf("FAIL: Received wrong message\n");
        jocky_socket_close(client);
        jocky_socket_close(server);
        return;
    }

    jocky_socket_close(client);
    jocky_socket_close(server);
    printf("PASS\n");
}

void test_socket_options() {
    printf("Test: Set socket options\n");
    jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);
    if (!sock) {
        printf("FAIL: Could not create socket\n");
        return;
    }

    if (jocky_socket_set_opt(sock, JOCKY_SO_REUSEADDR, 1) != 0) {
        printf("FAIL: Could not set SO_REUSEADDR\n");
        jocky_socket_close(sock);
        return;
    }

    if (jocky_socket_set_opt(sock, JOCKY_SO_KEEPALIVE, 1) != 0) {
        printf("FAIL: Could not set SO_KEEPALIVE\n");
        jocky_socket_close(sock);
        return;
    }

    if (jocky_socket_set_opt(sock, JOCKY_SO_SNDBUF, 65536) != 0) {
        printf("FAIL: Could not set SO_SNDBUF\n");
        jocky_socket_close(sock);
        return;
    }

    jocky_socket_close(sock);
    printf("PASS\n");
}

void test_nonblocking() {
    printf("Test: Set socket to non-blocking\n");
    jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);
    if (!sock) {
        printf("FAIL: Could not create socket\n");
        return;
    }

    if (jocky_socket_set_nonblocking(sock, 1) != 0) {
        printf("FAIL: Could not set non-blocking\n");
        jocky_socket_close(sock);
        return;
    }

    if (jocky_socket_set_nonblocking(sock, 0) != 0) {
        printf("FAIL: Could not set blocking\n");
        jocky_socket_close(sock);
        return;
    }

    jocky_socket_close(sock);
    printf("PASS\n");
}

void test_null_socket() {
    printf("Test: Operations on null socket\n");

    if (jocky_socket_close(NULL) == 0) {
        printf("FAIL: Should reject close on null\n");
        return;
    }

    if (jocky_socket_connect(NULL, "127.0.0.1", 9001) == 0) {
        printf("FAIL: Should reject connect on null\n");
        return;
    }

    if (jocky_socket_bind(NULL, "127.0.0.1", 9001) == 0) {
        printf("FAIL: Should reject bind on null\n");
        return;
    }

    printf("PASS\n");
}

int main() {
    printf("Network Tests\n");
    printf("==============\n\n");

    test_socket_create_ipv4();
    test_socket_create_ipv6();
    test_socket_create_udp();
    test_invalid_family();
    test_invalid_type();
    test_tcp_connection();
    test_udp_sendto_recvfrom();
    test_socket_options();
    test_nonblocking();
    test_null_socket();

    printf("\n==============\n");
    printf("All tests completed\n");

    return 0;
}
