#include "../include/jocky_dir.h"
#include "../include/jocky_file.h"
#include "../include/jocky_syscall.h"
#include <string.h>
#include <stdio.h>

/* Linux dirent64 structure */
struct linux_dirent64 {
    unsigned long   d_ino;
    long            d_off;
    unsigned short  d_reclen;
    unsigned char   d_type;
    char            d_name[256];
};

#define DT_DIR  4
#define DT_REG  8
#define DT_LNK  10

int jocky_mkdir(const char* path, int mode) {
    if (!path) return -1;
    long result = jocky_syscall2(SYS_mkdir, (long)path, mode);
    return (result == 0) ? 0 : -1;
}

int jocky_rmdir(const char* path) {
    if (!path) return -1;
    long result = jocky_syscall1(SYS_rmdir, (long)path);
    return (result == 0) ? 0 : -1;
}

int jocky_remove(const char* path) {
    if (!path) return -1;

    /* Check if it's a directory */
    int is_dir = jocky_is_directory(path);
    if (is_dir == 1) {
        return jocky_rmdir(path);
    } else if (is_dir == 0) {
        return jocky_unlink(path);
    }

    return -1;
}

int jocky_listdir(const char* path, jocky_dir_entry_t* entries, size_t max_entries) {
    if (!path || !entries || max_entries == 0) return -1;

    long fd = jocky_syscall2(SYS_open, (long)path, 0);
    if (fd < 0) return -1;

    char buffer[8192];
    int entry_count = 0;

    while (entry_count < (int)max_entries) {
        long nread = jocky_syscall3(SYS_getdents64, fd, (long)buffer, sizeof(buffer));
        if (nread < 0) {
            jocky_close(fd);
            return -1;
        }

        if (nread == 0) break;  /* End of directory */

        long offset = 0;
        while (offset < nread && entry_count < (int)max_entries) {
            struct linux_dirent64* de = (struct linux_dirent64*)(buffer + offset);

            /* Skip "." and ".." */
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
                strncpy(entries[entry_count].name, de->d_name, sizeof(entries[entry_count].name) - 1);
                entries[entry_count].name[sizeof(entries[entry_count].name) - 1] = '\0';
                entries[entry_count].is_dir = (de->d_type == DT_DIR) ? 1 : 0;
                entries[entry_count].is_symlink = (de->d_type == DT_LNK) ? 1 : 0;
                entry_count++;
            }

            offset += de->d_reclen;
        }
    }

    jocky_close(fd);
    return entry_count;
}

int jocky_dir_exists(const char* path) {
    if (!path) return -1;
    return jocky_is_directory(path);
}
