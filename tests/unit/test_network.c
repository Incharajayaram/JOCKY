#include "../../src/runtime/include/jocky_network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

/* Test 1: Socket creation and destruction */
int test_socket_create_destroy(void) {
    printf("TEST 1: Socket creation and destruction\n");

    jocky_socket_init();

    jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);
    if (!sock) {
        printf("  FAIL: Could not create socket\n");
        return 0;
    }

    if (jocky_socket_close(sock) != 0) {
        printf("  FAIL: Could not close socket\n");
        return 0;
    }

    jocky_socket_cleanup();
    printf("  PASS\n");
    return 1;
}

/* Test 2: UDP socket creation */
int test_udp_socket_create(void) {
    printf("TEST 2: UDP socket creation\n");

    jocky_socket_init();

    jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_DGRAM);
    if (!sock) {
        printf("  FAIL: Could not create UDP socket\n");
        jocky_socket_cleanup();
        return 0;
    }

    jocky_socket_close(sock);
    jocky_socket_cleanup();
    printf("  PASS\n");
    return 1;
}

/* Test 3: Invalid socket family */
int test_invalid_socket_family(void) {
    printf("TEST 3: Invalid socket family\n");

    jocky_socket_init();

    if (jocky_socket_create(999, JOCKY_SOCK_STREAM) != NULL) {
        printf("  FAIL: Should reject invalid family\n");
        jocky_socket_cleanup();
        return 0;
    }

    jocky_socket_cleanup();
    printf("  PASS\n");
    return 1;
}

/* Test 4: Socket bind and getsockname */
int test_socket_bind_getsockname(void) {
    printf("TEST 4: Socket bind and getsockname\n");

    jocky_socket_init();

    jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);
    if (!sock) {
        printf("  FAIL: Could not create socket\n");
        jocky_socket_cleanup();
        return 0;
    }

    /* Enable SO_REUSEADDR */
    jocky_socket_setopt(sock, JOCKY_SO_REUSEADDR, 1);

    if (jocky_socket_bind(sock, "127.0.0.1", 0) != 0) {
        printf("  FAIL: Could not bind to socket\n");
        jocky_socket_close(sock);
        jocky_socket_cleanup();
        return 0;
    }

    char host_buffer[256];
    int port = 0;
    if (jocky_socket_getsockname(sock, host_buffer, sizeof(host_buffer), &port) != 0) {
        printf("  FAIL: Could not get socket name\n");
        jocky_socket_close(sock);
        jocky_socket_cleanup();
        return 0;
    }

    if (port <= 0) {
        printf("  FAIL: Invalid port from getsockname\n");
        jocky_socket_close(sock);
        jocky_socket_cleanup();
        return 0;
    }

    jocky_socket_close(sock);
    jocky_socket_cleanup();
    printf("  PASS (bound to %s:%d)\n", host_buffer, port);
    return 1;
}

/* Helper: TCP server thread */
typedef struct {
    jocky_socket_t server_sock;
    int ready;
    int port;
} server_context_t;

void* tcp_server_thread(void* arg) {
    server_context_t* ctx = (server_context_t*)arg;

    jocky_socket_setopt(ctx->server_sock, JOCKY_SO_REUSEADDR, 1);
    jocky_socket_bind(ctx->server_sock, "127.0.0.1", 0);

    char host[256];
    jocky_socket_getsockname(ctx->server_sock, host, sizeof(host), &ctx->port);

    ctx->ready = 1;

    jocky_socket_listen(ctx->server_sock, 1);
    jocky_socket_t client = jocky_socket_accept(ctx->server_sock);

    if (client) {
        uint8_t buffer[256];
        int received = jocky_socket_recv(client, buffer, sizeof(buffer));
        if (received > 0) {
            jocky_socket_send(client, buffer, received);
        }
        jocky_socket_close(client);
    }

    return NULL;
}

/* Test 5: TCP echo server and client */
int test_tcp_echo(void) {
    printf("TEST 5: TCP echo server and client\n");

    jocky_socket_init();

    jocky_socket_t server_sock = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);
    if (!server_sock) {
        printf("  FAIL: Could not create server socket\n");
        jocky_socket_cleanup();
        return 0;
    }

    server_context_t ctx = {server_sock, 0, 0};

    pthread_t server_thread_id;
    pthread_create(&server_thread_id, NULL, tcp_server_thread, &ctx);

    /* Wait for server to be ready */
    int wait_count = 0;
    while (!ctx.ready && wait_count < 100) {
        struct timespec ts = {0, 10000000};  /* 10ms */
        nanosleep(&ts, NULL);
        wait_count++;
    }

    if (!ctx.ready) {
        printf("  FAIL: Server did not start\n");
        jocky_socket_close(server_sock);
        jocky_socket_cleanup();
        return 0;
    }

    /* Client: connect and send */
    jocky_socket_t client = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);
    if (!client) {
        printf("  FAIL: Could not create client socket\n");
        jocky_socket_close(server_sock);
        jocky_socket_cleanup();
        return 0;
    }

    if (jocky_socket_connect(client, "127.0.0.1", ctx.port) != 0) {
        printf("  FAIL: Could not connect\n");
        jocky_socket_close(client);
        jocky_socket_close(server_sock);
        jocky_socket_cleanup();
        return 0;
    }

    uint8_t send_data[] = "Hello, TCP!";
    int sent = jocky_socket_send(client, send_data, sizeof(send_data));

    if (sent != (int)sizeof(send_data)) {
        printf("  FAIL: Send mismatch\n");
        jocky_socket_close(client);
        jocky_socket_close(server_sock);
        jocky_socket_cleanup();
        return 0;
    }

    uint8_t recv_buffer[256];
    int received = jocky_socket_recv(client, recv_buffer, sizeof(recv_buffer));

    if (received != (int)sizeof(send_data) ||
        memcmp(send_data, recv_buffer, sizeof(send_data)) != 0) {
        printf("  FAIL: Echo mismatch\n");
        jocky_socket_close(client);
        jocky_socket_close(server_sock);
        jocky_socket_cleanup();
        return 0;
    }

    jocky_socket_close(client);
    jocky_socket_close(server_sock);

    pthread_join(server_thread_id, NULL);

    jocky_socket_cleanup();
    printf("  PASS\n");
    return 1;
}

/* Test 6: UDP sendto/recvfrom */
int test_udp_sendto_recvfrom(void) {
    printf("TEST 6: UDP sendto/recvfrom\n");

    jocky_socket_init();

    /* Create UDP server socket */
    jocky_socket_t server = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_DGRAM);
    if (!server) {
        printf("  FAIL: Could not create server socket\n");
        jocky_socket_cleanup();
        return 0;
    }

    jocky_socket_setopt(server, JOCKY_SO_REUSEADDR, 1);
    jocky_socket_bind(server, "127.0.0.1", 0);

    char host[256];
    int port = 0;
    jocky_socket_getsockname(server, host, sizeof(host), &port);

    /* Create client socket */
    jocky_socket_t client = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_DGRAM);
    if (!client) {
        printf("  FAIL: Could not create client socket\n");
        jocky_socket_close(server);
        jocky_socket_cleanup();
        return 0;
    }

    /* Send data from client to server */
    uint8_t send_data[] = "UDP Hello";
    int sent = jocky_socket_sendto(client, send_data, sizeof(send_data),
                                   "127.0.0.1", port);

    if (sent != (int)sizeof(send_data)) {
        printf("  FAIL: sendto failed (sent %d bytes)\n", sent);
        jocky_socket_close(client);
        jocky_socket_close(server);
        jocky_socket_cleanup();
        return 0;
    }

    /* Receive on server */
    uint8_t recv_buffer[256];
    char peer_host[256];
    int peer_port = 0;
    int received = jocky_socket_recvfrom(server, recv_buffer, sizeof(recv_buffer),
                                        peer_host, sizeof(peer_host), &peer_port);

    if (received != (int)sizeof(send_data) ||
        memcmp(send_data, recv_buffer, sizeof(send_data)) != 0) {
        printf("  FAIL: recvfrom mismatch\n");
        jocky_socket_close(client);
        jocky_socket_close(server);
        jocky_socket_cleanup();
        return 0;
    }

    jocky_socket_close(client);
    jocky_socket_close(server);
    jocky_socket_cleanup();
    printf("  PASS\n");
    return 1;
}

/* Test 7: Socket options */
int test_socket_options(void) {
    printf("TEST 7: Socket options (SO_REUSEADDR, SO_KEEPALIVE)\n");

    jocky_socket_init();

    jocky_socket_t sock = jocky_socket_create(JOCKY_AF_INET, JOCKY_SOCK_STREAM);
    if (!sock) {
        printf("  FAIL: Could not create socket\n");
        jocky_socket_cleanup();
        return 0;
    }

    if (jocky_socket_setopt(sock, JOCKY_SO_REUSEADDR, 1) != 0) {
        printf("  FAIL: Could not set SO_REUSEADDR\n");
        jocky_socket_close(sock);
        jocky_socket_cleanup();
        return 0;
    }

    int val = jocky_socket_getopt(sock, JOCKY_SO_REUSEADDR);
    if (val == -1) {
        printf("  FAIL: Could not get SO_REUSEADDR\n");
        jocky_socket_close(sock);
        jocky_socket_cleanup();
        return 0;
    }

    jocky_socket_close(sock);
    jocky_socket_cleanup();
    printf("  PASS\n");
    return 1;
}

/* Test 8: Hostname resolution */
int test_hostname_resolution(void) {
    printf("TEST 8: Hostname resolution\n");

    jocky_socket_init();

    char ip_buffer[256];
    if (jocky_socket_resolve_host("localhost", ip_buffer, sizeof(ip_buffer)) != 0) {
        printf("  FAIL: Could not resolve localhost\n");
        jocky_socket_cleanup();
        return 0;
    }

    if (strlen(ip_buffer) == 0) {
        printf("  FAIL: Empty hostname resolution\n");
        jocky_socket_cleanup();
        return 0;
    }

    jocky_socket_cleanup();
    printf("  PASS (localhost -> %s)\n", ip_buffer);
    return 1;
}

/* Test 9: IPv4 address parsing */
int test_ipv4_aton(void) {
    printf("TEST 9: IPv4 address parsing (inet_aton)\n");

    uint8_t addr[4];
    if (jocky_socket_aton("192.168.1.1", JOCKY_AF_INET, addr, sizeof(addr)) != 0) {
        printf("  FAIL: Could not parse IPv4 address\n");
        return 0;
    }

    if (addr[0] != 192 || addr[1] != 168 || addr[2] != 1 || addr[3] != 1) {
        printf("  FAIL: IPv4 address mismatch\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("JOCKY Network Tests\n");
    printf("========================================\n\n");

    int passed = 0;
    int total = 0;

    #define RUN_TEST(fn) do { \
        total++; \
        if (fn()) passed++; \
        printf("\n"); \
    } while(0)

    RUN_TEST(test_socket_create_destroy);
    RUN_TEST(test_udp_socket_create);
    RUN_TEST(test_invalid_socket_family);
    RUN_TEST(test_socket_bind_getsockname);
    RUN_TEST(test_tcp_echo);
    RUN_TEST(test_udp_sendto_recvfrom);
    RUN_TEST(test_socket_options);
    RUN_TEST(test_hostname_resolution);
    RUN_TEST(test_ipv4_aton);

    printf("========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n");

    return (passed == total) ? 0 : 1;
}
