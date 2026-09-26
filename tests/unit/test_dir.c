#include "../../src/runtime/include/jocky_dir.h"
#include "../../src/runtime/include/jocky_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_DIR "/tmp/jocky_testdir"
#define TEST_SUBDIR "/tmp/jocky_testdir/subdir"
#define TEST_FILE "/tmp/jocky_testdir/testfile.txt"

void cleanup(void) {
    jocky_unlink(TEST_FILE);
    jocky_rmdir(TEST_SUBDIR);
    jocky_rmdir(TEST_DIR);
}

int test_mkdir(void) {
    printf("TEST 1: mkdir\n");
    cleanup();

    if (jocky_mkdir(TEST_DIR, 0755) != 0) {
        printf("  FAIL: mkdir failed\n");
        return 0;
    }

    if (!jocky_dir_exists(TEST_DIR)) {
        printf("  FAIL: Directory should exist\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_dir_exists(void) {
    printf("TEST 2: dir_exists\n");

    if (!jocky_dir_exists(TEST_DIR)) {
        printf("  FAIL: Directory should exist\n");
        return 0;
    }

    int nonexist = jocky_dir_exists("/nonexistent/jocky/directory");
    if (nonexist == 1) {
        printf("  FAIL: Nonexistent directory should not exist (got %d)\n", nonexist);
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_mkdir_nested(void) {
    printf("TEST 3: mkdir nested\n");

    if (jocky_mkdir(TEST_SUBDIR, 0755) != 0) {
        printf("  FAIL: nested mkdir failed\n");
        return 0;
    }

    if (!jocky_dir_exists(TEST_SUBDIR)) {
        printf("  FAIL: Nested directory should exist\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_listdir(void) {
    printf("TEST 4: listdir\n");

    /* Create some test files */
    long fd = jocky_open(TEST_FILE, JOCKY_O_CREAT | JOCKY_O_WRONLY, 0644);
    if (fd >= 0) {
        jocky_write(fd, "test", 4);
        jocky_close(fd);
    }

    jocky_dir_entry_t entries[64];
    int count = jocky_listdir(TEST_DIR, entries, 64);

    if (count < 1) {
        printf("  FAIL: listdir returned %d entries\n", count);
        return 0;
    }

    /* Check if we can find subdir and testfile */
    int found_subdir = 0;
    int found_file = 0;

    for (int i = 0; i < count; i++) {
        if (strcmp(entries[i].name, "subdir") == 0 && entries[i].is_dir) {
            found_subdir = 1;
        }
        if (strcmp(entries[i].name, "testfile.txt") == 0 && !entries[i].is_dir) {
            found_file = 1;
        }
    }

    if (!found_subdir || !found_file) {
        printf("  FAIL: Expected entries not found (found_subdir=%d, found_file=%d)\n",
               found_subdir, found_file);
        return 0;
    }

    printf("  PASS (found %d entries including subdir and testfile.txt)\n", count);
    return 1;
}

int test_rmdir(void) {
    printf("TEST 5: rmdir\n");

    int rmdir_result = jocky_rmdir(TEST_SUBDIR);
    if (rmdir_result != 0) {
        printf("  FAIL: rmdir returned %d\n", rmdir_result);
        return 0;
    }

    int exists = jocky_dir_exists(TEST_SUBDIR);
    if (exists == 1) {
        printf("  FAIL: Directory should be removed (exists=%d)\n", exists);
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_remove_file(void) {
    printf("TEST 6: remove file\n");

    if (jocky_remove(TEST_FILE) != 0) {
        printf("  FAIL: remove failed\n");
        return 0;
    }

    if (jocky_file_exists(TEST_FILE)) {
        printf("  FAIL: File should be removed\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_remove_directory(void) {
    printf("TEST 7: remove directory\n");

    int remove_result = jocky_remove(TEST_DIR);
    if (remove_result != 0) {
        printf("  FAIL: remove returned %d\n", remove_result);
        return 0;
    }

    int exists = jocky_dir_exists(TEST_DIR);
    if (exists == 1) {
        printf("  FAIL: Directory should be removed (exists=%d)\n", exists);
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("JOCKY Directory Operations Tests\n");
    printf("========================================\n\n");

    int passed = 0;
    int total = 0;

    #define RUN_TEST(fn) do { \
        total++; \
        if (fn()) passed++; \
        printf("\n"); \
    } while(0)

    RUN_TEST(test_mkdir);
    RUN_TEST(test_dir_exists);
    RUN_TEST(test_mkdir_nested);
    RUN_TEST(test_listdir);
    RUN_TEST(test_rmdir);
    RUN_TEST(test_remove_file);
    RUN_TEST(test_remove_directory);

    cleanup();

    printf("========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n");

    return (passed == total) ? 0 : 1;
}
