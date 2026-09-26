/**
 * JOCKY File I/O Runtime API
 *
 * Provides platform-independent file I/O operations for JOCKY programs.
 * Supports Windows and Linux file operations.
 */

#ifndef JOCKY_IO_H
#define JOCKY_IO_H

#include <stdint.h>
#include <stddef.h>

typedef void* jocky_file_t;

#define JOCKY_FILE_READ    0x01
#define JOCKY_FILE_WRITE   0x02
#define JOCKY_FILE_APPEND  0x04
#define JOCKY_FILE_BINARY  0x08

#define JOCKY_SEEK_SET     0
#define JOCKY_SEEK_CUR     1
#define JOCKY_SEEK_END     2

/**
 * Open a file for reading, writing, or both.
 *
 * @param path File path (absolute or relative)
 * @param flags JOCKY_FILE_READ, JOCKY_FILE_WRITE, JOCKY_FILE_APPEND, JOCKY_FILE_BINARY
 * @return File handle on success, NULL on failure
 *
 * Example:
 *   jocky_file_t f = jocky_fopen("C:\\output.txt", JOCKY_FILE_WRITE | JOCKY_FILE_BINARY);
 */
jocky_file_t jocky_fopen(const char* path, uint32_t flags);

/**
 * Close an open file handle.
 *
 * @param f File handle from jocky_fopen
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_fclose(jocky_file_t f);

/**
 * Read data from a file.
 *
 * @param f File handle
 * @param buf Output buffer (must be allocated)
 * @param count Number of bytes to read
 * @return Number of bytes read, or -1 on error
 */
int64_t jocky_fread(jocky_file_t f, void* buf, uint64_t count);

/**
 * Write data to a file.
 *
 * @param f File handle
 * @param buf Data to write
 * @param count Number of bytes to write
 * @return Number of bytes written, or -1 on error
 */
int64_t jocky_fwrite(jocky_file_t f, const void* buf, uint64_t count);

/**
 * Seek to position in file.
 *
 * @param f File handle
 * @param offset Offset from origin
 * @param origin JOCKY_SEEK_SET, JOCKY_SEEK_CUR, or JOCKY_SEEK_END
 * @return New file position, or -1 on error
 */
int64_t jocky_fseek(jocky_file_t f, int64_t offset, uint32_t origin);

/**
 * Get current file position.
 *
 * @param f File handle
 * @return Current position, or -1 on error
 */
int64_t jocky_ftell(jocky_file_t f);

/**
 * Get file size.
 *
 * @param path File path
 * @return File size in bytes, or -1 on error
 */
int64_t jocky_fsize(const char* path);

/**
 * Check if file exists.
 *
 * @param path File path
 * @return 1 if exists, 0 if not, -1 on error
 */
int32_t jocky_fexists(const char* path);

/**
 * Delete a file.
 *
 * @param path File path
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_fdelete(const char* path);

/**
 * Rename a file.
 *
 * @param old_path Current file path
 * @param new_path New file path
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_frename(const char* old_path, const char* new_path);

/**
 * Read entire file into memory.
 *
 * @param path File path
 * @param out_size Output parameter for file size
 * @return Pointer to allocated buffer (must be freed with jocky_free), NULL on error
 */
void* jocky_fread_all(const char* path, uint64_t* out_size);

/**
 * Write data to file atomically (create temp, then rename).
 *
 * @param path Target file path
 * @param data Data to write
 * @param size Size of data
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_fwrite_atomic(const char* path, const void* data, uint64_t size);

/**
 * Flush file buffer to disk.
 *
 * @param f File handle
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_fflush(jocky_file_t f);

#endif /* JOCKY_IO_H */
