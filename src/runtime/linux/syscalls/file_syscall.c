#include "../include/jocky_file.h"
#include "../include/jocky_syscall.h"
#include <string.h>
#include <stdio.h>

/* ========== FILE OPERATIONS ========== */

long jocky_open(const char* path, int flags, int mode) {
    if (!path) return -1;
    return jocky_syscall3(SYS_open, (long)path, flags, mode);
}

int jocky_close(long fd) {
    if (fd < 0) return -1;
    long result = jocky_syscall1(SYS_close, fd);
    return (result == 0) ? 0 : -1;
}

long jocky_read(long fd, void* buffer, size_t size) {
    if (fd < 0 || !buffer || size == 0) return -1;
    return jocky_syscall3(SYS_read, fd, (long)buffer, size);
}

long jocky_write(long fd, const void* buffer, size_t size) {
    if (fd < 0 || !buffer || size == 0) return -1;
    return jocky_syscall3(SYS_write, fd, (long)buffer, size);
}

long jocky_lseek(long fd, long offset, int whence) {
    if (fd < 0) return -1;
    return jocky_syscall3(SYS_lseek, fd, offset, whence);
}

long jocky_tell(long fd) {
    if (fd < 0) return -1;
    return jocky_syscall3(SYS_lseek, fd, 0, 1);  /* SEEK_CUR = 1 */
}

int jocky_truncate(const char* path, size_t size) {
    if (!path) return -1;
    long result = jocky_syscall2(SYS_truncate, (long)path, size);
    return (result == 0) ? 0 : -1;
}

/* Linux stat structure (x86_64) */
struct linux_stat {
    unsigned long   st_dev;
    unsigned long   st_ino;
    unsigned long   st_nlink;
    unsigned int    st_mode;
    unsigned int    st_uid;
    unsigned int    st_gid;
    unsigned long   st_rdev;
    long            st_size;
    long            st_blksize;
    long            st_blocks;
    long            st_atime;
    long            st_atime_nsec;
    long            st_mtime;
    long            st_mtime_nsec;
    long            st_ctime;
    long            st_ctime_nsec;
    long            __unused[3];
};

/* S_IFMT = 0170000, S_IFDIR = 0040000, S_IFLNK = 0120000 */

int jocky_stat(const char* path, jocky_stat_t* out) {
    if (!path || !out) return -1;

    struct linux_stat buf;
    long result = jocky_syscall2(SYS_stat, (long)path, (long)&buf);
    if (result != 0) return -1;

    out->size = buf.st_size;
    out->mode = buf.st_mode;
    out->uid = buf.st_uid;
    out->gid = buf.st_gid;
    out->mtime = buf.st_mtime;
    out->atime = buf.st_atime;
    out->ctime = buf.st_ctime;
    out->is_dir = (buf.st_mode & 0170000) == 0040000 ? 1 : 0;
    out->is_symlink = (buf.st_mode & 0170000) == 0120000 ? 1 : 0;

    return 0;
}

int jocky_lstat(const char* path, jocky_stat_t* out) {
    if (!path || !out) return -1;

    struct linux_stat buf;
    long result = jocky_syscall2(SYS_lstat, (long)path, (long)&buf);
    if (result != 0) return -1;

    out->size = buf.st_size;
    out->mode = buf.st_mode;
    out->uid = buf.st_uid;
    out->gid = buf.st_gid;
    out->mtime = buf.st_mtime;
    out->atime = buf.st_atime;
    out->ctime = buf.st_ctime;
    out->is_dir = (buf.st_mode & 0170000) == 0040000 ? 1 : 0;
    out->is_symlink = (buf.st_mode & 0170000) == 0120000 ? 1 : 0;

    return 0;
}

int jocky_fstat(long fd, jocky_stat_t* out) {
    if (fd < 0 || !out) return -1;

    struct linux_stat buf;
    long result = jocky_syscall2(SYS_fstat, fd, (long)&buf);
    if (result != 0) return -1;

    out->size = buf.st_size;
    out->mode = buf.st_mode;
    out->uid = buf.st_uid;
    out->gid = buf.st_gid;
    out->mtime = buf.st_mtime;
    out->atime = buf.st_atime;
    out->ctime = buf.st_ctime;
    out->is_dir = (buf.st_mode & 0170000) == 0040000 ? 1 : 0;
    out->is_symlink = (buf.st_mode & 0170000) == 0120000 ? 1 : 0;

    return 0;
}

int jocky_file_exists(const char* path) {
    if (!path) return -1;

    struct linux_stat buf;
    long result = jocky_syscall2(SYS_stat, (long)path, (long)&buf);
    return (result == 0) ? 1 : 0;
}

uint64_t jocky_file_size(const char* path) {
    if (!path) return 0;

    jocky_stat_t st;
    if (jocky_stat(path, &st) != 0) return 0;
    return st.size;
}

int jocky_is_directory(const char* path) {
    if (!path) return -1;

    jocky_stat_t st;
    if (jocky_stat(path, &st) != 0) return -1;
    return st.is_dir;
}

int jocky_is_symlink(const char* path) {
    if (!path) return -1;

    jocky_stat_t st;
    if (jocky_lstat(path, &st) != 0) return -1;
    return st.is_symlink;
}

/* ========== FILE MANIPULATION ========== */

int jocky_unlink(const char* path) {
    if (!path) return -1;
    long result = jocky_syscall1(SYS_unlink, (long)path);
    return (result == 0) ? 0 : -1;
}

int jocky_rename(const char* oldpath, const char* newpath) {
    if (!oldpath || !newpath) return -1;
    long result = jocky_syscall2(SYS_rename, (long)oldpath, (long)newpath);
    return (result == 0) ? 0 : -1;
}

int jocky_symlink(const char* target, const char* linkpath) {
    if (!target || !linkpath) return -1;
    long result = jocky_syscall2(SYS_symlink, (long)target, (long)linkpath);
    return (result == 0) ? 0 : -1;
}

int jocky_readlink(const char* path, char* buffer, size_t size) {
    if (!path || !buffer || size == 0) return -1;

    long result = jocky_syscall3(SYS_readlink, (long)path, (long)buffer, size - 1);
    if (result <= 0) return -1;

    buffer[result] = '\0';
    return 0;
}

int jocky_chmod(const char* path, int mode) {
    if (!path) return -1;
    long result = jocky_syscall2(SYS_chmod, (long)path, mode);
    return (result == 0) ? 0 : -1;
}

int jocky_chown(const char* path, int uid, int gid) {
    if (!path) return -1;
    long result = jocky_syscall3(SYS_chown, (long)path, uid, gid);
    return (result == 0) ? 0 : -1;
}
