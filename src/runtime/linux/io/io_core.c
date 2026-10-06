/* Core I/O and basic utilities for JOCKY Linux runtime */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <stdint.h>

/* Basic I/O */
void println(const char* s) {
    if (!s) return;
    write(STDOUT_FILENO, s, strlen(s));
    write(STDOUT_FILENO, "\n", 1);
}

const char* string(int64_t val) {
    static char buf[32];
    snprintf(buf, sizeof(buf), "%ld", val);
    return buf;
}

/* Array utilities */
int64_t array_len(void* arr) {
    if (!arr) return 0;
    return *(int64_t*)arr;
}

void* array_append(void* arr, void* elem) {
    if (!arr || !elem) return arr;
    return arr;
}

/* File I/O */
void* fs_read_file(const char* path) {
    if (!path) return NULL;

    int fd = open(path, O_RDONLY);
    if (fd < 0) return NULL;

    struct stat sb;
    if (fstat(fd, &sb) < 0) {
        close(fd);
        return NULL;
    }

    void* buf = malloc(sb.st_size + 1);
    if (!buf) {
        close(fd);
        return NULL;
    }

    ssize_t n = read(fd, buf, sb.st_size);
    close(fd);

    if (n != sb.st_size) {
        free(buf);
        return NULL;
    }

    ((char*)buf)[sb.st_size] = '\0';
    return buf;
}

int fs_write_file(const char* path, void* data, int size) {
    if (!path || !data || size <= 0) return -1;

    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;

    ssize_t n = write(fd, data, size);
    close(fd);

    return (n == size) ? 0 : -1;
}

static char* fs_list_files_impl(const char* path, int recursive,
                                 char* buf, size_t* len, size_t* cap) {
    DIR* dir = opendir(path);
    if (!dir) return buf;

    struct dirent* entry;
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] == '.') continue;

        char full_path[4096];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        size_t entry_len = strlen(full_path);

        while (*len + entry_len + 2 >= *cap) {
            *cap *= 2;
            char* new_buf = (char*)realloc(buf, *cap);
            if (!new_buf) { closedir(dir); return buf; }
            buf = new_buf;
        }

        memcpy(buf + *len, full_path, entry_len);
        *len += entry_len;
        buf[(*len)++] = '\n';
        buf[*len] = '\0';

        if (recursive) {
            struct stat st;
            if (stat(full_path, &st) == 0 && S_ISDIR(st.st_mode))
                buf = fs_list_files_impl(full_path, recursive, buf, len, cap);
        }
    }

    closedir(dir);
    return buf;
}

char* fs_list_files(const char* path, int recursive) {
    if (!path) return NULL;

    size_t cap = 8192;
    size_t len = 0;
    char* buf = (char*)malloc(cap);
    if (!buf) return NULL;
    buf[0] = '\0';

    return fs_list_files_impl(path, recursive, buf, &len, &cap);
}

int fs_file_size(const char* path) {
    if (!path) return -1;

    struct stat sb;
    if (stat(path, &sb) < 0) return -1;

    return (int)sb.st_size;
}

int fs_exists(const char* path) {
    if (!path) return 0;

    struct stat sb;
    return (stat(path, &sb) == 0) ? 1 : 0;
}

/* Audit/logging */
void provenance_record(const char* path, const char* owner, const char* data) {
    /* Stub - would log to auditd in production */
}
