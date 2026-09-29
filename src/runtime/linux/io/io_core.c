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

void* fs_list_files(const char* path, int recursive) {
    if (!path) return NULL;

    DIR* dir = opendir(path);
    if (!dir) return NULL;

    char** files = malloc(sizeof(char*) * 256);
    int count = 0;

    struct dirent* entry;
    while ((entry = readdir(dir)) && count < 255) {
        if (entry->d_name[0] == '.') continue;
        files[count++] = strdup(entry->d_name);
    }

    closedir(dir);
    files[count] = NULL;

    return files;
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
