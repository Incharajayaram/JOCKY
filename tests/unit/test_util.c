#include "../../src/runtime/include/jocky_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_time(void) {
    printf("TEST 1: get current time\n");

    long t1 = jocky_time();
    if (t1 <= 0) {
        printf("  FAIL: Invalid time: %ld\n", t1);
        return 0;
    }

    /* Sleep a bit */
    jocky_sleep_ms(10);

    long t2 = jocky_time();
    if (t2 < t1) {
        printf("  FAIL: Time went backwards: %ld -> %ld\n", t1, t2);
        return 0;
    }

    printf("  PASS (time: %ld, diff: %ld)\n", t1, t2 - t1);
    return 1;
}

int test_sleep(void) {
    printf("TEST 2: sleep milliseconds\n");

    long t1 = jocky_time();
    int result = jocky_sleep_ms(100);
    long t2 = jocky_time();

    if (result != 0) {
        printf("  FAIL: sleep returned %d\n", result);
        return 0;
    }

    /* Note: sleep might not be exact, just check it slept at least a little */
    if (t2 == t1) {
        printf("  WARN: Sleep didn't advance time\n");
        /* Don't fail */
    }

    printf("  PASS\n");
    return 1;
}

int test_strlen(void) {
    printf("TEST 3: strlen\n");

    const char* str = "Hello, JOCKY!";
    size_t len = jocky_strlen(str);

    if (len != 13) {
        printf("  FAIL: Expected 13, got %zu\n", len);
        return 0;
    }

    printf("  PASS (len: %zu)\n", len);
    return 1;
}

int test_strcmp(void) {
    printf("TEST 4: strcmp\n");

    int cmp1 = jocky_strcmp("abc", "abc");
    if (cmp1 != 0) {
        printf("  FAIL: Equal strings returned %d\n", cmp1);
        return 0;
    }

    int cmp2 = jocky_strcmp("abc", "def");
    if (cmp2 >= 0) {
        printf("  FAIL: Lesser string comparison wrong\n");
        return 0;
    }

    int cmp3 = jocky_strcmp("xyz", "abc");
    if (cmp3 <= 0) {
        printf("  FAIL: Greater string comparison wrong\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_strncmp(void) {
    printf("TEST 5: strncmp\n");

    int cmp1 = jocky_strncmp("abcdef", "abcxyz", 3);
    if (cmp1 != 0) {
        printf("  FAIL: Equal first 3 chars\n");
        return 0;
    }

    int cmp2 = jocky_strncmp("abcdef", "abcxyz", 6);
    if (cmp2 == 0) {
        printf("  FAIL: Full strings should differ\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_strchr(void) {
    printf("TEST 6: strchr\n");

    const char* str = "Hello, World!";
    char* found = jocky_strchr(str, 'o');

    if (!found) {
        printf("  FAIL: Could not find 'o'\n");
        return 0;
    }

    if (*found != 'o') {
        printf("  FAIL: Found wrong character\n");
        return 0;
    }

    char* notfound = jocky_strchr(str, 'x');
    if (notfound != NULL) {
        printf("  FAIL: Should not find 'x'\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_strrchr(void) {
    printf("TEST 7: strrchr\n");

    const char* str = "Hello, World!";
    char* found = jocky_strrchr(str, 'o');

    if (!found) {
        printf("  FAIL: Could not find 'o'\n");
        return 0;
    }

    if (*found != 'o') {
        printf("  FAIL: Found wrong character\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_strncpy_safe(void) {
    printf("TEST 8: strncpy_safe\n");

    char buffer[16];
    int result = jocky_strncpy_safe(buffer, "Hello", 16);

    if (result != 0) {
        printf("  FAIL: strncpy_safe returned %d\n", result);
        return 0;
    }

    if (strcmp(buffer, "Hello") != 0) {
        printf("  FAIL: String mismatch\n");
        return 0;
    }

    /* Test buffer overflow */
    int result2 = jocky_strncpy_safe(buffer, "This is a very long string", 16);
    if (result2 == 0) {
        printf("  FAIL: Should detect overflow\n");
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_strncat_safe(void) {
    printf("TEST 9: strncat_safe\n");

    char buffer[32];
    jocky_strncpy_safe(buffer, "Hello", 32);

    int result = jocky_strncat_safe(buffer, ", World!", 32);
    if (result != 0) {
        printf("  FAIL: strncat_safe returned %d\n", result);
        return 0;
    }

    if (strcmp(buffer, "Hello, World!") != 0) {
        printf("  FAIL: String mismatch: got '%s'\n", buffer);
        return 0;
    }

    printf("  PASS\n");
    return 1;
}

int test_errno(void) {
    printf("TEST 10: errno handling\n");

    jocky_set_errno(0);
    int e1 = jocky_get_errno();
    if (e1 != 0) {
        printf("  FAIL: errno not cleared\n");
        return 0;
    }

    jocky_set_errno(5);
    int e2 = jocky_get_errno();
    if (e2 != 5) {
        printf("  FAIL: errno not set correctly\n");
        return 0;
    }

    const char* errmsg = jocky_strerror(e2);
    if (!errmsg) {
        printf("  FAIL: strerror returned NULL\n");
        return 0;
    }

    printf("  PASS (errno: %d, message: %s)\n", e2, errmsg);
    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("JOCKY Utility Functions Tests\n");
    printf("========================================\n\n");

    int passed = 0;
    int total = 0;

    #define RUN_TEST(fn) do { \
        total++; \
        if (fn()) passed++; \
        printf("\n"); \
    } while(0)

    RUN_TEST(test_time);
    RUN_TEST(test_sleep);
    RUN_TEST(test_strlen);
    RUN_TEST(test_strcmp);
    RUN_TEST(test_strncmp);
    RUN_TEST(test_strchr);
    RUN_TEST(test_strrchr);
    RUN_TEST(test_strncpy_safe);
    RUN_TEST(test_strncat_safe);
    RUN_TEST(test_errno);

    printf("========================================\n");
    printf("Results: %d/%d tests passed\n", passed, total);
    printf("========================================\n");

    return (passed == total) ? 0 : 1;
}
