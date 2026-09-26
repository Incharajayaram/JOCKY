/**
 * JOCKY File I/O Implementation
 *
 * Cross-platform file operations for Windows and Linux.
 */

#include "io.h"
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include <io.h>

typedef struct {
    HANDLE handle;
    uint32_t flags;
} JockyFile;

jocky_file_t jocky_fopen(const char* path, uint32_t flags) {
    if (!path) return NULL;

    DWORD access = 0;
    DWORD creation = 0;

    if (flags & JOCKY_FILE_READ && flags & JOCKY_FILE_WRITE) {
        access = GENERIC_READ | GENERIC_WRITE;
    } else if (flags & JOCKY_FILE_WRITE) {
        access = GENERIC_WRITE;
    } else {
        access = GENERIC_READ;
    }

    if (flags & JOCKY_FILE_APPEND) {
        creation = OPEN_ALWAYS;
    } else if (flags & JOCKY_FILE_WRITE) {
        creation = CREATE_ALWAYS;
    } else {
        creation = OPEN_EXISTING;
    }

    // Convert path to wide char for Windows
    int wlen = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
    wchar_t* wpath = malloc(wlen * sizeof(wchar_t));
    if (!wpath) return NULL;

    MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, wlen);

    HANDLE h = CreateFileW(wpath, access, FILE_SHARE_READ, NULL, creation, 0, NULL);
    free(wpath);

    if (h == INVALID_HANDLE_VALUE) return NULL;

    // If append mode, seek to end
    if (flags & JOCKY_FILE_APPEND) {
        SetFilePointer(h, 0, NULL, FILE_END);
    }

    JockyFile* jf = malloc(sizeof(JockyFile));
    if (!jf) {
        CloseHandle(h);
        return NULL;
    }

    jf->handle = h;
    jf->flags = flags;

    return (jocky_file_t)jf;
}

int32_t jocky_fclose(jocky_file_t f) {
    if (!f) return -1;

    JockyFile* jf = (JockyFile*)f;
    BOOL result = CloseHandle(jf->handle);
    free(jf);

    return result ? 0 : -1;
}

int64_t jocky_fread(jocky_file_t f, void* buf, uint64_t count) {
    if (!f || !buf) return -1;

    JockyFile* jf = (JockyFile*)f;
    DWORD read = 0;

    // ReadFile has 32-bit limit, so we need to handle large reads
    if (count > 0x7FFFFFFF) count = 0x7FFFFFFF;

    BOOL result = ReadFile(jf->handle, buf, (DWORD)count, &read, NULL);
    return result ? (int64_t)read : -1;
}

int64_t jocky_fwrite(jocky_file_t f, const void* buf, uint64_t count) {
    if (!f || !buf) return -1;

    JockyFile* jf = (JockyFile*)f;
    DWORD written = 0;

    // WriteFile has 32-bit limit
    if (count > 0x7FFFFFFF) count = 0x7FFFFFFF;

    BOOL result = WriteFile(jf->handle, buf, (DWORD)count, &written, NULL);
    return result ? (int64_t)written : -1;
}

int64_t jocky_fseek(jocky_file_t f, int64_t offset, uint32_t origin) {
    if (!f) return -1;

    JockyFile* jf = (JockyFile*)f;
    DWORD move_method = FILE_BEGIN;

    switch (origin) {
        case JOCKY_SEEK_SET: move_method = FILE_BEGIN; break;
        case JOCKY_SEEK_CUR: move_method = FILE_CURRENT; break;
        case JOCKY_SEEK_END: move_method = FILE_END; break;
        default: return -1;
    }

    LARGE_INTEGER li;
    li.QuadPart = offset;

    LARGE_INTEGER result;
    if (!SetFilePointerEx(jf->handle, li, &result, move_method)) {
        return -1;
    }

    return result.QuadPart;
}

int64_t jocky_ftell(jocky_file_t f) {
    if (!f) return -1;

    JockyFile* jf = (JockyFile*)f;

    LARGE_INTEGER zero;
    zero.QuadPart = 0;

    LARGE_INTEGER result;
    if (!SetFilePointerEx(jf->handle, zero, &result, FILE_CURRENT)) {
        return -1;
    }

    return result.QuadPart;
}

int64_t jocky_fsize(const char* path) {
    if (!path) return -1;

    // Convert to wide char
    int wlen = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
    wchar_t* wpath = malloc(wlen * sizeof(wchar_t));
    if (!wpath) return -1;

    MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, wlen);

    WIN32_FILE_ATTRIBUTE_DATA fad;
    BOOL result = GetFileAttributesExW(wpath, GetFileExInfoStandard, &fad);
    free(wpath);

    if (!result) return -1;

    LARGE_INTEGER size;
    size.LowPart = fad.nFileSizeLow;
    size.HighPart = fad.nFileSizeHigh;

    return size.QuadPart;
}

int32_t jocky_fexists(const char* path) {
    if (!path) return -1;

    int wlen = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
    wchar_t* wpath = malloc(wlen * sizeof(wchar_t));
    if (!wpath) return -1;

    MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, wlen);

    DWORD attr = GetFileAttributesW(wpath);
    free(wpath);

    if (attr == INVALID_FILE_ATTRIBUTES) return 0;
    if (attr & FILE_ATTRIBUTE_DIRECTORY) return 0;  // Is directory
    return 1;
}

int32_t jocky_fdelete(const char* path) {
    if (!path) return -1;

    int wlen = MultiByteToWideChar(CP_UTF8, 0, path, -1, NULL, 0);
    wchar_t* wpath = malloc(wlen * sizeof(wchar_t));
    if (!wpath) return -1;

    MultiByteToWideChar(CP_UTF8, 0, path, -1, wpath, wlen);

    BOOL result = DeleteFileW(wpath);
    free(wpath);

    return result ? 0 : -1;
}

int32_t jocky_frename(const char* old_path, const char* new_path) {
    if (!old_path || !new_path) return -1;

    int wlen1 = MultiByteToWideChar(CP_UTF8, 0, old_path, -1, NULL, 0);
    wchar_t* wold = malloc(wlen1 * sizeof(wchar_t));
    if (!wold) return -1;

    int wlen2 = MultiByteToWideChar(CP_UTF8, 0, new_path, -1, NULL, 0);
    wchar_t* wnew = malloc(wlen2 * sizeof(wchar_t));
    if (!wnew) {
        free(wold);
        return -1;
    }

    MultiByteToWideChar(CP_UTF8, 0, old_path, -1, wold, wlen1);
    MultiByteToWideChar(CP_UTF8, 0, new_path, -1, wnew, wlen2);

    BOOL result = MoveFileExW(wold, wnew, MOVEFILE_REPLACE_EXISTING);

    free(wold);
    free(wnew);

    return result ? 0 : -1;
}

void* jocky_fread_all(const char* path, uint64_t* out_size) {
    if (!path || !out_size) return NULL;

    int64_t size = jocky_fsize(path);
    if (size <= 0) return NULL;

    jocky_file_t f = jocky_fopen(path, JOCKY_FILE_READ | JOCKY_FILE_BINARY);
    if (!f) return NULL;

    void* buf = malloc(size);
    if (!buf) {
        jocky_fclose(f);
        return NULL;
    }

    int64_t read = jocky_fread(f, buf, size);
    jocky_fclose(f);

    if (read != size) {
        free(buf);
        return NULL;
    }

    *out_size = size;
    return buf;
}

int32_t jocky_fwrite_atomic(const char* path, const void* data, uint64_t size) {
    if (!path || !data) return -1;

    // Create temporary file path
    char temp_path[MAX_PATH];
    strcpy_s(temp_path, sizeof(temp_path), path);
    strcat_s(temp_path, sizeof(temp_path), ".tmp");

    // Write to temporary file
    jocky_file_t f = jocky_fopen(temp_path, JOCKY_FILE_WRITE | JOCKY_FILE_BINARY);
    if (!f) return -1;

    int64_t written = jocky_fwrite(f, data, size);
    jocky_fclose(f);

    if (written != (int64_t)size) {
        jocky_fdelete(temp_path);
        return -1;
    }

    // Atomically rename
    return jocky_frename(temp_path, path);
}

int32_t jocky_fflush(jocky_file_t f) {
    if (!f) return -1;

    JockyFile* jf = (JockyFile*)f;
    BOOL result = FlushFileBuffers(jf->handle);

    return result ? 0 : -1;
}

#else  /* Linux implementation */

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

typedef struct {
    int fd;
    uint32_t flags;
} JockyFile;

jocky_file_t jocky_fopen(const char* path, uint32_t flags) {
    if (!path) return NULL;

    int oflags = 0;
    int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    if (flags & JOCKY_FILE_READ && flags & JOCKY_FILE_WRITE) {
        oflags = O_RDWR;
    } else if (flags & JOCKY_FILE_WRITE) {
        oflags = O_WRONLY;
    } else {
        oflags = O_RDONLY;
    }

    if (flags & JOCKY_FILE_WRITE) {
        oflags |= O_CREAT;
        if (!(flags & JOCKY_FILE_APPEND)) {
            oflags |= O_TRUNC;
        }
    }

    if (flags & JOCKY_FILE_APPEND) {
        oflags |= O_APPEND;
    }

    int fd = open(path, oflags, mode);
    if (fd < 0) return NULL;

    JockyFile* jf = malloc(sizeof(JockyFile));
    if (!jf) {
        close(fd);
        return NULL;
    }

    jf->fd = fd;
    jf->flags = flags;

    return (jocky_file_t)jf;
}

int32_t jocky_fclose(jocky_file_t f) {
    if (!f) return -1;

    JockyFile* jf = (JockyFile*)f;
    int result = close(jf->fd);
    free(jf);

    return result;
}

int64_t jocky_fread(jocky_file_t f, void* buf, uint64_t count) {
    if (!f || !buf) return -1;

    JockyFile* jf = (JockyFile*)f;
    return read(jf->fd, buf, count);
}

int64_t jocky_fwrite(jocky_file_t f, const void* buf, uint64_t count) {
    if (!f || !buf) return -1;

    JockyFile* jf = (JockyFile*)f;
    return write(jf->fd, buf, count);
}

int64_t jocky_fseek(jocky_file_t f, int64_t offset, uint32_t origin) {
    if (!f) return -1;

    JockyFile* jf = (JockyFile*)f;
    int whence = SEEK_SET;

    switch (origin) {
        case JOCKY_SEEK_SET: whence = SEEK_SET; break;
        case JOCKY_SEEK_CUR: whence = SEEK_CUR; break;
        case JOCKY_SEEK_END: whence = SEEK_END; break;
        default: return -1;
    }

    return lseek(jf->fd, offset, whence);
}

int64_t jocky_ftell(jocky_file_t f) {
    if (!f) return -1;

    JockyFile* jf = (JockyFile*)f;
    return lseek(jf->fd, 0, SEEK_CUR);
}

int64_t jocky_fsize(const char* path) {
    if (!path) return -1;

    struct stat st;
    if (stat(path, &st) < 0) return -1;

    return st.st_size;
}

int32_t jocky_fexists(const char* path) {
    if (!path) return -1;

    struct stat st;
    if (stat(path, &st) < 0) return 0;

    return S_ISREG(st.st_mode) ? 1 : 0;
}

int32_t jocky_fdelete(const char* path) {
    if (!path) return -1;
    return unlink(path);
}

int32_t jocky_frename(const char* old_path, const char* new_path) {
    if (!old_path || !new_path) return -1;
    return rename(old_path, new_path);
}

void* jocky_fread_all(const char* path, uint64_t* out_size) {
    if (!path || !out_size) return NULL;

    int64_t size = jocky_fsize(path);
    if (size <= 0) return NULL;

    jocky_file_t f = jocky_fopen(path, JOCKY_FILE_READ | JOCKY_FILE_BINARY);
    if (!f) return NULL;

    void* buf = malloc(size);
    if (!buf) {
        jocky_fclose(f);
        return NULL;
    }

    int64_t read = jocky_fread(f, buf, size);
    jocky_fclose(f);

    if (read != size) {
        free(buf);
        return NULL;
    }

    *out_size = size;
    return buf;
}

int32_t jocky_fwrite_atomic(const char* path, const void* data, uint64_t size) {
    if (!path || !data) return -1;

    // Create temporary file path
    char temp_path[PATH_MAX];
    snprintf(temp_path, sizeof(temp_path), "%s.tmp", path);

    // Write to temporary file
    jocky_file_t f = jocky_fopen(temp_path, JOCKY_FILE_WRITE | JOCKY_FILE_BINARY);
    if (!f) return -1;

    int64_t written = jocky_fwrite(f, data, size);
    jocky_fclose(f);

    if (written != (int64_t)size) {
        jocky_fdelete(temp_path);
        return -1;
    }

    // Atomically rename
    return jocky_frename(temp_path, path);
}

int32_t jocky_fflush(jocky_file_t f) {
    if (!f) return -1;

    JockyFile* jf = (JockyFile*)f;
    return fsync(jf->fd);
}

#endif

#include <limits.h>
