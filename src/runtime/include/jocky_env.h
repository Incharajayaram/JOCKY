#ifndef JOCKY_ENV_H
#define JOCKY_ENV_H

#include <stddef.h>

/**
 * Get environment variable value.
 * buffer must be at least 256 bytes.
 * Returns 0 on success, -1 if not found or error.
 */
int jocky_getenv(const char* name, char* buffer, size_t size);

/**
 * Set environment variable.
 * Returns 0 on success, -1 on error.
 */
int jocky_setenv(const char* name, const char* value, int overwrite);

/**
 * Unset environment variable.
 * Returns 0 on success, -1 on error.
 */
int jocky_unsetenv(const char* name);

/**
 * Get current user ID.
 * Returns UID (0 = root).
 */
long jocky_getuid(void);

/**
 * Get current group ID.
 * Returns GID.
 */
long jocky_getgid(void);

/**
 * Get username from UID.
 * buffer must be at least 256 bytes.
 * Returns 0 on success, -1 on error.
 */
int jocky_get_username(long uid, char* buffer, size_t size);

/**
 * Get group name from GID.
 * buffer must be at least 256 bytes.
 * Returns 0 on success, -1 on error.
 */
int jocky_get_groupname(long gid, char* buffer, size_t size);

/**
 * Get home directory for current user.
 * buffer must be at least 256 bytes.
 * Returns 0 on success, -1 on error.
 */
int jocky_get_home_dir(char* buffer, size_t size);

/**
 * Get current working directory.
 * buffer must be at least 256 bytes.
 * Returns 0 on success, -1 on error.
 */
int jocky_get_cwd(char* buffer, size_t size);

/**
 * Change working directory.
 * Returns 0 on success, -1 on error.
 */
int jocky_chdir(const char* path);

#endif // JOCKY_ENV_H
