#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <windows.h>

void println(const char* s) {
    if (!s) return;
    fputs(s, stdout);
    fputc('\n', stdout);
    fflush(stdout);
}

char* jocky_str_concat(const char* a, const char* b) {
    size_t len_a = a ? strlen(a) : 0;
    size_t len_b = b ? strlen(b) : 0;
    char* result = malloc(len_a + len_b + 1);
    if (result) {
        result[0] = '\0';
        if (a) strcpy(result, a);
        if (b) strcat(result, b);
        result[len_a + len_b] = '\0';
    }
    return result;
}

char* string(int64_t val) {
    char* buf = malloc(64);
    if (buf) {
        snprintf(buf, 64, "%lld", val);
    }
    return buf;
}

int64_t array_len(void* arr) {
    if (!arr) return 0;
    int64_t* len_ptr = (int64_t*)((uintptr_t)arr - sizeof(int64_t));
    return *len_ptr;
}

void* array_append(void* arr, void* elem) {
    if (!arr || !elem) return arr;

    int64_t* len_ptr = (int64_t*)((uintptr_t)arr - sizeof(int64_t));
    int64_t current_len = *len_ptr;
    size_t elem_size = 8;

    int64_t new_len = current_len + 1;
    size_t new_total_size = sizeof(int64_t) + (new_len * elem_size);

    void* new_arr = realloc((void*)((uintptr_t)arr - sizeof(int64_t)), new_total_size);
    if (!new_arr) return arr;

    int64_t* new_len_ptr = (int64_t*)new_arr;
    *new_len_ptr = new_len;

    void* data = (void*)((uintptr_t)new_arr + sizeof(int64_t));
    memcpy((void*)((uintptr_t)data + (current_len * elem_size)), elem, elem_size);

    return data;
}
