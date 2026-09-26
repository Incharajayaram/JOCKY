/**
 * JOCKY Registry API (Windows Only)
 *
 * Provides access to Windows Registry for evasion and persistence.
 */

#ifndef JOCKY_REGISTRY_H
#define JOCKY_REGISTRY_H

#include <stdint.h>
#include <stddef.h>

typedef void* jocky_reg_handle_t;

/* Registry hives */
#define JOCKY_HKEY_CLASSES_ROOT     ((jocky_reg_handle_t)0x80000000)
#define JOCKY_HKEY_CURRENT_USER     ((jocky_reg_handle_t)0x80000001)
#define JOCKY_HKEY_LOCAL_MACHINE    ((jocky_reg_handle_t)0x80000002)
#define JOCKY_HKEY_USERS            ((jocky_reg_handle_t)0x80000003)
#define JOCKY_HKEY_PERFORMANCE_DATA ((jocky_reg_handle_t)0x80000004)
#define JOCKY_HKEY_CURRENT_CONFIG   ((jocky_reg_handle_t)0x80000005)

/* Registry value types */
#define JOCKY_REG_NONE              0
#define JOCKY_REG_SZ                1
#define JOCKY_REG_EXPAND_SZ         2
#define JOCKY_REG_BINARY            3
#define JOCKY_REG_DWORD             4
#define JOCKY_REG_DWORD_BIG_ENDIAN  5
#define JOCKY_REG_LINK              6
#define JOCKY_REG_MULTI_SZ          7
#define JOCKY_REG_RESOURCE_LIST     8
#define JOCKY_REG_FULL_RESOURCE_DESCRIPTOR 9
#define JOCKY_REG_RESOURCE_REQUIREMENTS_LIST 10
#define JOCKY_REG_QWORD             11

/* Access flags */
#define JOCKY_KEY_READ              0x20019
#define JOCKY_KEY_WRITE             0x20006
#define JOCKY_KEY_ALL_ACCESS        0xF003F

/**
 * Open a registry key.
 *
 * @param hkey Parent key handle (JOCKY_HKEY_*)
 * @param subkey Subkey path (e.g., "Software\\Microsoft\\Windows")
 * @param access Access flags (JOCKY_KEY_READ, JOCKY_KEY_WRITE, etc.)
 * @return Key handle on success, NULL on failure
 *
 * Example:
 *   jocky_reg_handle_t key = jocky_reg_open(JOCKY_HKEY_LOCAL_MACHINE,
 *     "System\\CurrentControlSet\\Services", JOCKY_KEY_READ);
 */
jocky_reg_handle_t jocky_reg_open(jocky_reg_handle_t hkey, const char* subkey, uint32_t access);

/**
 * Create a new registry key.
 *
 * @param hkey Parent key handle
 * @param subkey Subkey path to create
 * @param access Access flags
 * @return Key handle on success, NULL on failure
 */
jocky_reg_handle_t jocky_reg_create(jocky_reg_handle_t hkey, const char* subkey, uint32_t access);

/**
 * Close a registry key handle.
 *
 * @param key Key handle from jocky_reg_open/create
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_close(jocky_reg_handle_t key);

/**
 * Query a registry value.
 *
 * @param key Key handle
 * @param value_name Value name (e.g., "DisplayName")
 * @param type Output: value type (JOCKY_REG_*)
 * @param data Output buffer for value data
 * @param data_size Input: buffer size, Output: bytes read
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_query_value(jocky_reg_handle_t key, const char* value_name,
                             uint32_t* type, void* data, uint32_t* data_size);

/**
 * Set a registry value (create if doesn't exist).
 *
 * @param key Key handle
 * @param value_name Value name
 * @param type Value type (JOCKY_REG_*)
 * @param data Value data
 * @param data_size Size of data
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_set_value(jocky_reg_handle_t key, const char* value_name,
                           uint32_t type, const void* data, uint32_t data_size);

/**
 * Delete a registry value.
 *
 * @param key Key handle
 * @param value_name Value name
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_delete_value(jocky_reg_handle_t key, const char* value_name);

/**
 * Delete a registry key and all subkeys.
 *
 * @param hkey Parent key handle
 * @param subkey Subkey path to delete
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_delete_key(jocky_reg_handle_t hkey, const char* subkey);

/**
 * Enumerate subkeys under a key.
 *
 * @param key Key handle
 * @param index Subkey index (0-based)
 * @param name Output buffer for subkey name
 * @param name_size Input: buffer size, Output: name length
 * @return 0 on success, -1 if no more subkeys or on error
 */
int32_t jocky_reg_enum_key(jocky_reg_handle_t key, uint32_t index,
                          char* name, uint32_t* name_size);

/**
 * Enumerate values under a key.
 *
 * @param key Key handle
 * @param index Value index (0-based)
 * @param name Output buffer for value name
 * @param name_size Input: buffer size, Output: name length
 * @param type Output: value type
 * @return 0 on success, -1 if no more values or on error
 */
int32_t jocky_reg_enum_value(jocky_reg_handle_t key, uint32_t index,
                            char* name, uint32_t* name_size, uint32_t* type);

/**
 * Get the number of subkeys under a key.
 *
 * @param key Key handle
 * @return Number of subkeys, or -1 on error
 */
int32_t jocky_reg_key_count(jocky_reg_handle_t key);

/**
 * Get the number of values under a key.
 *
 * @param key Key handle
 * @return Number of values, or -1 on error
 */
int32_t jocky_reg_value_count(jocky_reg_handle_t key);

/**
 * Query a DWORD (uint32_t) value.
 *
 * @param key Key handle
 * @param value_name Value name
 * @param out_value Output: DWORD value
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_query_dword(jocky_reg_handle_t key, const char* value_name, uint32_t* out_value);

/**
 * Set a DWORD value.
 *
 * @param key Key handle
 * @param value_name Value name
 * @param value DWORD value
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_set_dword(jocky_reg_handle_t key, const char* value_name, uint32_t value);

/**
 * Query a string value.
 *
 * @param key Key handle
 * @param value_name Value name
 * @param buffer Output buffer
 * @param buffer_size Buffer size
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_query_string(jocky_reg_handle_t key, const char* value_name,
                              char* buffer, uint32_t buffer_size);

/**
 * Set a string value.
 *
 * @param key Key handle
 * @param value_name Value name
 * @param value String value
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_set_string(jocky_reg_handle_t key, const char* value_name, const char* value);

/**
 * Query binary value.
 *
 * @param key Key handle
 * @param value_name Value name
 * @param buffer Output buffer
 * @param buffer_size Input: buffer size, Output: bytes read
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_query_binary(jocky_reg_handle_t key, const char* value_name,
                              void* buffer, uint32_t* buffer_size);

/**
 * Set binary value.
 *
 * @param key Key handle
 * @param value_name Value name
 * @param data Binary data
 * @param data_size Data size
 * @return 0 on success, non-zero on failure
 */
int32_t jocky_reg_set_binary(jocky_reg_handle_t key, const char* value_name,
                            const void* data, uint32_t data_size);

#endif /* JOCKY_REGISTRY_H */
