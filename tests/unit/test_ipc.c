#include "../../src/runtime/include/jocky_ipc.h"
#include "../../src/runtime/include/jocky_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int test_pipe_create(void) {
    printf("TEST 1: create pipe\n");

    long fds[2] = {-1, -1};
    int result = jocky_pipe(fds);

    printf("  DEBUG: result=%d, fds[0]=%ld, fds[1]=%ld\n", result, fds[0], fds[1]);

    if (result != 0) {
        printf("  FAIL: pipe() failed with result %d\n", result);
        return 0;
    }

    if (fds[0] < 0 || fds[1] < 0 || fds[0] > 1024 || fds[1] > 1024) {
        printf("  FAIL: Invalid file descriptors: %ld, %ld\n", fds[0], fds[1]);
        return 0;
    }

    if (fds[0] >= 0) jocky_close(fds[0]);
    if (fds[1] >= 0) jocky_close(fds[1]);

    printf("  PASS (fds: %ld, %ld)\n", fds[0], fds[1]);
    return 1;
}

int test_pipe_write_read(void) {
    printf("TEST 2: pipe write and read\n");

    long fds[2];
    if (jocky_pipe(fds) != 0) {
        printf("  FAIL: pipe() failed\n");
        return 0;
    }

    const char* data = "Hello, pipe!";
    long written = jocky_write(fds[1], data, strlen(data));

    if (written != (long)strlen(data)) {
        printf("  FAIL: Write failed\n");
        jocky_close(fds[0]);
        jocky_close(fds[1]);
        return 0;
    }

    /* Close write end to signal EOF */
    jocky_close(fds[1]);

    char buffer[256];
    long nread = jocky_read(fds[0], buffer, sizeof(buffer) - 1);
    jocky_close(fds[0]);

    if (nread != (long)strlen(data)) {
        printf("  FAIL: Read returned %ld, expected %zu\n", nread, strlen(data));
        return 0;
    }

    buffer[nread] = '\0';

    if (strcmp(buffer, data) != 0) {
        printf("  FAIL: Data mismatch: got '%s', expected '%s'\n", buffer, data);
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_pipe2_create(void) {
    printf("TEST 3: create pipe2 with flags\n");

    long fds[2];
    int result = jocky_pipe2(fds, 0);

    if (result != 0) {
        printf("  FAIL: pipe2() failed\n");
        return 0;
    }

    if (fds[0] < 0 || fds[1] < 0) {
        printf("  FAIL: Invalid file descriptors\n");
        return 0;
    }

    jocky_close(fds[0]);
    jocky_close(fds[1]);

    printf("  PASS\n");
    return 1;
}

int test_socket_create(void) {
    printf("TEST 4: create socket\n");

    long sockfd = jocky_socket(JOCKY_AF_UNIX, JOCKY_SOCK_STREAM, 0);

    if (sockfd < 0) {
        printf("  FAIL: socket() failed\n");
        return 0;
    }

    jocky_close(sockfd);

    printf("  PASS (sockfd: %ld)\n", sockfd);
    return 1;
}

int test_inet_socket_create(void) {
    printf("TEST 5: create IPv4 socket\n");

    long sockfd = jocky_socket(JOCKY_AF_INET, JOCKY_SOCK_STREAM, 0);

    if (sockfd < 0) {
        printf("  FAIL: socket() failed\n");
        return 0;
    }

    jocky_close(sockfd);

    printf("  PASS (sockfd: %ld)\n", sockfd);
    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("JOCKY IPC Tests\n");
    printf("========================================\n\n");

    int passed = 0;
    int total = 0;

    #define RUN_TEST(fn) do { \
        total++; \
        if (fn()) passed++; \
        printf("\n"); \
    } while(0)

    RUN_TEST(test_pipe_create);
    RUN_TEST(test_pipe_write_read);
    RUN_TEST(test_pipe2_create);
    RUN_TEST(test_socket_create);
    RUN_TEST(test_inet_socket_create);

    printf("========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n");

    return (passed == total) ? 0 : 1;
}
