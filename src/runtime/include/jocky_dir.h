#ifndef JOCKY_DIR_H
#define JOCKY_DIR_H

#include <stddef.h>

/* Directory entry structure */
typedef struct {
    char name[256];     /* Entry name */
    int is_dir;         /* 1 if directory, 0 if file */
    int is_symlink;     /* 1 if symlink, 0 otherwise */
} jocky_dir_entry_t;

/* ========== DIRECTORY OPERATIONS ========== */

/**
 * Create a directory.
 * mode: Permission bits (0755 for directories)
 * Returns 0 on success, -1 on error.
 */
int jocky_mkdir(const char* path, int mode);

/**
 * Remove an empty directory.
 * Returns 0 on success, -1 on error.
 */
int jocky_rmdir(const char* path);

/**
 * Remove a file or empty directory.
 * Returns 0 on success, -1 on error.
 */
int jocky_remove(const char* path);

/**
 * List directory entries.
 * entries: Output array of entries
 * max_entries: Maximum number of entries to read
 * Returns number of entries, -1 on error.
 */
int jocky_listdir(const char* path, jocky_dir_entry_t* entries, size_t max_entries);

/**
 * Check if directory exists.
 * Returns 1 if exists, 0 if not, -1 on error.
 */
int jocky_dir_exists(const char* path);

#endif // JOCKY_DIR_H
