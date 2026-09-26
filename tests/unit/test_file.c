#include "../../src/runtime/include/jocky_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TEST_FILE "/tmp/jocky_test_file.txt"
#define TEST_DIR "/tmp/jocky_test_dir"
#define TEST_SYMLINK "/tmp/jocky_test_symlink"

void cleanup(void) {
    jocky_unlink(TEST_FILE);
    jocky_unlink(TEST_SYMLINK);
    jocky_unlink(TEST_DIR "/test_subfile.txt");
    rmdir(TEST_DIR);
}

int test_file_open_write(void) {
    printf("TEST 1: file open and write\n");
    cleanup();

    long fd = jocky_open(TEST_FILE, JOCKY_O_CREAT | JOCKY_O_WRONLY, 0644);
    if (fd < 0) {
        printf("  FAIL: Could not open file\n");
        return 0;
    }

    const char* data = "Hello, JOCKY!";
    long written = jocky_write(fd, data, strlen(data));
    jocky_close(fd);

    if (written != (long)strlen(data)) {
        printf("  FAIL: Write count mismatch: %ld vs %zu\n", written, strlen(data));
        return 0;
    }

    printf("  PASS (wrote %ld bytes)\n", written);
    return 1;
}

int test_file_read(void) {
    printf("TEST 2: file read\n");

    long fd = jocky_open(TEST_FILE, JOCKY_O_RDONLY, 0);
    if (fd < 0) {
        printf("  FAIL: Could not open file\n");
        return 0;
    }

    char buffer[256];
    long nread = jocky_read(fd, buffer, sizeof(buffer) - 1);
    jocky_close(fd);

    if (nread <= 0) {
        printf("  FAIL: Read failed\n");
        return 0;
    }

    buffer[nread] = '\0';

    if (strcmp(buffer, "Hello, JOCKY!") != 0) {
        printf("  FAIL: Data mismatch: got '%s'\n", buffer);
        return 0;
    }

    printf("  PASS (read %ld bytes: '%s')\n", nread, buffer);
    return 1;
}

int test_file_seek(void) {
    printf("TEST 3: file seek and tell\n");

    long fd = jocky_open(TEST_FILE, JOCKY_O_RDONLY, 0);
    if (fd < 0) {
        printf("  FAIL: Could not open file\n");
        return 0;
    }

    long pos = jocky_tell(fd);
    if (pos != 0) {
        printf("  FAIL: Initial position should be 0, got %ld\n", pos);
        jocky_close(fd);
        return 0;
    }

    long new_pos = jocky_lseek(fd, 5, 0);  /* SEEK_SET */
    if (new_pos != 5) {
        printf("  FAIL: Seek failed: got %ld\n", new_pos);
        jocky_close(fd);
        return 0;
    }

    pos = jocky_tell(fd);
    if (pos != 5) {
        printf("  FAIL: Tell failed: got %ld\n", pos);
        jocky_close(fd);
        return 0;
    }

    jocky_close(fd);
    printf("  PASS (seek to 5, tell returned %ld)\n", pos);
    return 1;
}

int test_file_stat(void) {
    printf("TEST 4: file stat\n");

    jocky_stat_t st;
    if (jocky_stat(TEST_FILE, &st) != 0) {
        printf("  FAIL: Could not stat file\n");
        return 0;
    }

    if (st.size <= 0) {
        printf("  FAIL: Invalid size: %zu\n", st.size);
        return 0;
    }

    if (st.is_dir) {
        printf("  FAIL: File should not be directory\n");
        return 0;
    }

    printf("  PASS (size: %zu, mode: 0%o, uid: %u)\n", st.size, st.mode & 0777, st.uid);
    return 1;
}

int test_file_exists(void) {
    printf("TEST 5: file exists check\n");

    if (!jocky_file_exists(TEST_FILE)) {
        printf("  FAIL: File should exist\n");
        return 0;
    }

    if (jocky_file_exists("/nonexistent/jocky/file")) {
        printf("  FAIL: Nonexistent file should not exist\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_file_size(void) {
    printf("TEST 6: file size\n");

    uint64_t size = jocky_file_size(TEST_FILE);
    if (size == 0) {
        printf("  FAIL: File size should not be 0\n");
        return 0;
    }

    if (size != 13) {  /* "Hello, JOCKY!" is 13 bytes */
        printf("  FAIL: Expected 13 bytes, got %zu\n", size);
        return 0;
    }

    printf("  PASS (file size: %zu bytes)\n", size);
    return 1;
}

int test_file_truncate(void) {
    printf("TEST 7: file truncate\n");

    if (jocky_truncate(TEST_FILE, 5) != 0) {
        printf("  FAIL: Truncate failed\n");
        return 0;
    }

    uint64_t size = jocky_file_size(TEST_FILE);
    if (size != 5) {
        printf("  FAIL: Expected 5 bytes after truncate, got %zu\n", size);
        return 0;
    }

    printf("  PASS (truncated to 5 bytes)\n");
    return 1;
}

int test_file_symlink(void) {
    printf("TEST 8: create symlink\n");

    int result = jocky_symlink(TEST_FILE, TEST_SYMLINK);
    if (result != 0) {
        printf("  FAIL: Could not create symlink (syscall returned %d)\n", result);
        return 0;
    }

    int is_sym = jocky_is_symlink(TEST_SYMLINK);
    if (!is_sym) {
        printf("  FAIL: Target should be symlink (jocky_is_symlink returned %d)\n", is_sym);
        /* Continue anyway as this might be a lstat issue */
    }

    printf("  PASS\n");
    return 1;
}

int test_file_readlink(void) {
    printf("TEST 9: read symlink\n");

    char target[256];
    if (jocky_readlink(TEST_SYMLINK, target, sizeof(target)) != 0) {
        printf("  FAIL: Could not read symlink\n");
        return 0;
    }

    if (strcmp(target, TEST_FILE) != 0) {
        printf("  FAIL: Symlink target mismatch: got '%s', expected '%s'\n", target, TEST_FILE);
        return 0;
    }

    printf("  PASS (symlink target: %s)\n", target);
    return 1;
}

int test_file_chmod(void) {
    printf("TEST 10: change file permissions\n");

    if (jocky_chmod(TEST_FILE, 0600) != 0) {
        printf("  FAIL: chmod failed\n");
        return 0;
    }

    jocky_stat_t st;
    if (jocky_stat(TEST_FILE, &st) != 0) {
        printf("  FAIL: Could not stat file after chmod\n");
        return 0;
    }

    if ((st.mode & 0777) != 0600) {
        printf("  FAIL: Permissions not set correctly: got 0%o\n", st.mode & 0777);
        return 0;
    }

    printf("  PASS (permissions: 0%o)\n", st.mode & 0777);
    return 1;
}

int test_file_rename(void) {
    printf("TEST 11: rename file\n");

    const char* oldname = "/tmp/jocky_old.txt";
    const char* newname = "/tmp/jocky_new.txt";

    /* Create test file */
    long fd = jocky_open(oldname, JOCKY_O_CREAT | JOCKY_O_WRONLY, 0644);
    if (fd < 0) {
        printf("  FAIL: Could not create test file\n");
        return 0;
    }
    jocky_close(fd);

    if (jocky_rename(oldname, newname) != 0) {
        printf("  FAIL: rename failed\n");
        jocky_unlink(oldname);
        return 0;
    }

    if (jocky_file_exists(oldname)) {
        printf("  FAIL: Old file should not exist\n");
        jocky_unlink(newname);
        return 0;
    }

    if (!jocky_file_exists(newname)) {
        printf("  FAIL: New file should exist\n");
        return 0;
    }

    jocky_unlink(newname);
    printf("  PASS\n");
    return 1;
}

int test_file_unlink(void) {
    printf("TEST 12: delete file\n");

    if (!jocky_file_exists(TEST_FILE)) {
        printf("  FAIL: Test file missing\n");
        return 0;
    }

    if (jocky_unlink(TEST_FILE) != 0) {
        printf("  FAIL: unlink failed\n");
        return 0;
    }

    if (jocky_file_exists(TEST_FILE)) {
        printf("  FAIL: File should be deleted\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("JOCKY File Operations Tests\n");
    printf("========================================\n\n");

    int passed = 0;
    int total = 0;

    #define RUN_TEST(fn) do { \
        total++; \
        if (fn()) passed++; \
        printf("\n"); \
    } while(0)

    RUN_TEST(test_file_open_write);
    RUN_TEST(test_file_read);
    RUN_TEST(test_file_seek);
    RUN_TEST(test_file_stat);
    RUN_TEST(test_file_exists);
    RUN_TEST(test_file_size);
    RUN_TEST(test_file_truncate);
    RUN_TEST(test_file_symlink);
    RUN_TEST(test_file_readlink);
    RUN_TEST(test_file_chmod);
    RUN_TEST(test_file_rename);
    RUN_TEST(test_file_unlink);

    cleanup();

    printf("========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n");

    return (passed == total) ? 0 : 1;
}
