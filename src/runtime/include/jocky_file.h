#ifndef JOCKY_FILE_H
#define JOCKY_FILE_H

#include <stddef.h>
#include <stdint.h>

/* File operation flags */
#define JOCKY_O_RDONLY   0
#define JOCKY_O_WRONLY   1
#define JOCKY_O_RDWR     2
#define JOCKY_O_CREAT    64
#define JOCKY_O_TRUNC    512
#define JOCKY_O_APPEND   1024

/* File stat structure */
typedef struct {
    uint64_t size;              /* File size in bytes */
    uint32_t mode;              /* Permission bits */
    uint32_t uid;               /* Owner UID */
    uint32_t gid;               /* Owner GID */
    uint64_t mtime;             /* Modification time (seconds since epoch) */
    uint64_t atime;             /* Access time */
    uint64_t ctime;             /* Change time */
    int is_dir;                 /* 1 if directory, 0 otherwise */
    int is_symlink;             /* 1 if symlink, 0 otherwise */
} jocky_stat_t;

/* ========== FILE OPERATIONS ========== */

/**
 * Open a file.
 * flags: JOCKY_O_* constants
 * mode: Permission bits (0644 for regular files)
 * Returns file descriptor (>= 0), or -1 on error.
 */
long jocky_open(const char* path, int flags, int mode);

/**
 * Close a file descriptor.
 * Returns 0 on success, -1 on error.
 */
int jocky_close(long fd);

/**
 * Read from file descriptor.
 * Returns number of bytes read, 0 on EOF, -1 on error.
 */
long jocky_read(long fd, void* buffer, size_t size);

/**
 * Write to file descriptor.
 * Returns number of bytes written, -1 on error.
 */
long jocky_write(long fd, const void* buffer, size_t size);

/**
 * Seek in file.
 * whence: 0=beginning, 1=current, 2=end
 * Returns new offset, -1 on error.
 */
long jocky_lseek(long fd, long offset, int whence);

/**
 * Get current file position.
 * Returns current offset, -1 on error.
 */
long jocky_tell(long fd);

/**
 * Truncate file to size.
 * Returns 0 on success, -1 on error.
 */
int jocky_truncate(const char* path, size_t size);

/**
 * Stat a file (follows symlinks).
 * Returns 0 on success, -1 on error.
 */
int jocky_stat(const char* path, jocky_stat_t* out);

/**
 * Stat a file without following symlinks (lstat).
 * Returns 0 on success, -1 on error.
 */
int jocky_lstat(const char* path, jocky_stat_t* out);

/**
 * Stat via file descriptor (fstat).
 * Returns 0 on success, -1 on error.
 */
int jocky_fstat(long fd, jocky_stat_t* out);

/**
 * Check if file exists.
 * Returns 1 if exists, 0 if not, -1 on error.
 */
int jocky_file_exists(const char* path);

/**
 * Get file size in bytes.
 * Returns size, 0 if not found or error.
 */
uint64_t jocky_file_size(const char* path);

/**
 * Check if path is a directory.
 * Returns 1 if directory, 0 if not, -1 on error.
 */
int jocky_is_directory(const char* path);

/**
 * Check if path is a symlink.
 * Returns 1 if symlink, 0 if not, -1 on error.
 */
int jocky_is_symlink(const char* path);

/* ========== FILE MANIPULATION ========== */

/**
 * Delete a file.
 * Returns 0 on success, -1 on error.
 */
int jocky_unlink(const char* path);

/**
 * Rename/move a file.
 * Returns 0 on success, -1 on error.
 */
int jocky_rename(const char* oldpath, const char* newpath);

/**
 * Create a symlink.
 * Returns 0 on success, -1 on error.
 */
int jocky_symlink(const char* target, const char* linkpath);

/**
 * Read symlink target.
 * buffer must be at least 256 bytes.
 * Returns 0 on success, -1 on error.
 */
int jocky_readlink(const char* path, char* buffer, size_t size);

/**
 * Change file permissions.
 * Returns 0 on success, -1 on error.
 */
int jocky_chmod(const char* path, int mode);

/**
 * Change file owner.
 * Returns 0 on success, -1 on error.
 */
int jocky_chown(const char* path, int uid, int gid);

#endif // JOCKY_FILE_H
