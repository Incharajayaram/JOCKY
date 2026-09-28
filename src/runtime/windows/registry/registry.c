/**
 * JOCKY Registry Implementation (Windows Only)
 */

#include "registry.h"
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>

static HKEY jocky_to_win_key(jocky_reg_handle_t key) {
    return (HKEY)key;
}

static DWORD jocky_to_win_access(uint32_t access) {
    DWORD win_access = 0;

    if (access & JOCKY_KEY_READ) win_access |= KEY_READ;
    if (access & JOCKY_KEY_WRITE) win_access |= KEY_WRITE;
    if (access & JOCKY_KEY_ALL_ACCESS) win_access |= KEY_ALL_ACCESS;

    return win_access ?: KEY_READ;
}

jocky_reg_handle_t jocky_reg_open(jocky_reg_handle_t hkey, const char* subkey, uint32_t access) {
    if (!subkey) return NULL;

    HKEY parent = jocky_to_win_key(hkey);
    DWORD win_access = jocky_to_win_access(access);
    HKEY result;

    // Convert UTF-8 to UTF-16
    int wlen = MultiByteToWideChar(CP_UTF8, 0, subkey, -1, NULL, 0);
    wchar_t* wsubkey = malloc(wlen * sizeof(wchar_t));
    if (!wsubkey) return NULL;

    MultiByteToWideChar(CP_UTF8, 0, subkey, -1, wsubkey, wlen);

    LONG status = RegOpenKeyExW(parent, wsubkey, 0, win_access, &result);
    free(wsubkey);

    return (status == ERROR_SUCCESS) ? (jocky_reg_handle_t)result : NULL;
}

jocky_reg_handle_t jocky_reg_create(jocky_reg_handle_t hkey, const char* subkey, uint32_t access) {
    if (!subkey) return NULL;

    HKEY parent = jocky_to_win_key(hkey);
    DWORD win_access = jocky_to_win_access(access);
    HKEY result;
    DWORD disposition;

    // Convert UTF-8 to UTF-16
    int wlen = MultiByteToWideChar(CP_UTF8, 0, subkey, -1, NULL, 0);
    wchar_t* wsubkey = malloc(wlen * sizeof(wchar_t));
    if (!wsubkey) return NULL;

    MultiByteToWideChar(CP_UTF8, 0, subkey, -1, wsubkey, wlen);

    LONG status = RegCreateKeyExW(parent, wsubkey, 0, NULL, REG_OPTION_NON_VOLATILE,
                                  win_access, NULL, &result, &disposition);
    free(wsubkey);

    return (status == ERROR_SUCCESS) ? (jocky_reg_handle_t)result : NULL;
}

int32_t jocky_reg_close(jocky_reg_handle_t key) {
    if (!key) return -1;

    HKEY hkey = jocky_to_win_key(key);
    LONG status = RegCloseKey(hkey);

    return (status == ERROR_SUCCESS) ? 0 : -1;
}

int32_t jocky_reg_query_value(jocky_reg_handle_t key, const char* value_name,
                             uint32_t* type, void* data, uint32_t* data_size) {
    if (!key || !value_name || !data_size) return -1;

    HKEY hkey = jocky_to_win_key(key);
    DWORD reg_type = REG_NONE;
    DWORD size = *data_size;

    // Convert value name to wide char
    int wlen = MultiByteToWideChar(CP_UTF8, 0, value_name, -1, NULL, 0);
    wchar_t* wname = malloc(wlen * sizeof(wchar_t));
    if (!wname) return -1;

    MultiByteToWideChar(CP_UTF8, 0, value_name, -1, wname, wlen);

    LONG status = RegQueryValueExW(hkey, wname, NULL, &reg_type, (LPBYTE)data, &size);
    free(wname);

    if (status == ERROR_SUCCESS) {
        if (type) *type = reg_type;
        *data_size = size;
        return 0;
    }

    return -1;
}

int32_t jocky_reg_set_value(jocky_reg_handle_t key, const char* value_name,
                           uint32_t type, const void* data, uint32_t data_size) {
    if (!key || !value_name || !data) return -1;

    HKEY hkey = jocky_to_win_key(key);

    // Convert value name to wide char
    int wlen = MultiByteToWideChar(CP_UTF8, 0, value_name, -1, NULL, 0);
    wchar_t* wname = malloc(wlen * sizeof(wchar_t));
    if (!wname) return -1;

    MultiByteToWideChar(CP_UTF8, 0, value_name, -1, wname, wlen);

    LONG status = RegSetValueExW(hkey, wname, 0, type, (LPCBYTE)data, data_size);
    free(wname);

    return (status == ERROR_SUCCESS) ? 0 : -1;
}

int32_t jocky_reg_delete_value(jocky_reg_handle_t key, const char* value_name) {
    if (!key || !value_name) return -1;

    HKEY hkey = jocky_to_win_key(key);

    // Convert value name to wide char
    int wlen = MultiByteToWideChar(CP_UTF8, 0, value_name, -1, NULL, 0);
    wchar_t* wname = malloc(wlen * sizeof(wchar_t));
    if (!wname) return -1;

    MultiByteToWideChar(CP_UTF8, 0, value_name, -1, wname, wlen);

    LONG status = RegDeleteValueW(hkey, wname);
    free(wname);

    return (status == ERROR_SUCCESS) ? 0 : -1;
}

int32_t jocky_reg_delete_key(jocky_reg_handle_t hkey, const char* subkey) {
    if (!hkey || !subkey) return -1;

    HKEY parent = jocky_to_win_key(hkey);

    // Convert subkey to wide char
    int wlen = MultiByteToWideChar(CP_UTF8, 0, subkey, -1, NULL, 0);
    wchar_t* wsubkey = malloc(wlen * sizeof(wchar_t));
    if (!wsubkey) return -1;

    MultiByteToWideChar(CP_UTF8, 0, subkey, -1, wsubkey, wlen);

    LONG status = RegDeleteTreeW(parent, wsubkey);
    free(wsubkey);

    return (status == ERROR_SUCCESS) ? 0 : -1;
}

int32_t jocky_reg_enum_key(jocky_reg_handle_t key, uint32_t index,
                          char* name, uint32_t* name_size) {
    if (!key || !name_size) return -1;

    HKEY hkey = jocky_to_win_key(key);
    wchar_t wname[256];
    DWORD wsize = sizeof(wname) / sizeof(wchar_t);

    LONG status = RegEnumKeyExW(hkey, index, wname, &wsize, NULL, NULL, NULL, NULL);

    if (status != ERROR_SUCCESS) return -1;

    // Convert back to UTF-8
    int utf8_len = WideCharToMultiByte(CP_UTF8, 0, wname, -1, name, *name_size, NULL, NULL);
    if (utf8_len == 0) return -1;

    *name_size = utf8_len - 1;  // Exclude null terminator
    return 0;
}

int32_t jocky_reg_enum_value(jocky_reg_handle_t key, uint32_t index,
                            char* name, uint32_t* name_size, uint32_t* type) {
    if (!key || !name_size) return -1;

    HKEY hkey = jocky_to_win_key(key);
    wchar_t wname[256];
    DWORD wsize = sizeof(wname) / sizeof(wchar_t);
    DWORD reg_type = REG_NONE;

    LONG status = RegEnumValueW(hkey, index, wname, &wsize, NULL, &reg_type, NULL, NULL);

    if (status != ERROR_SUCCESS) return -1;

    // Convert back to UTF-8
    int utf8_len = WideCharToMultiByte(CP_UTF8, 0, wname, -1, name, *name_size, NULL, NULL);
    if (utf8_len == 0) return -1;

    if (type) *type = reg_type;
    *name_size = utf8_len - 1;
    return 0;
}

int32_t jocky_reg_key_count(jocky_reg_handle_t key) {
    if (!key) return -1;

    HKEY hkey = jocky_to_win_key(key);
    DWORD subkeys = 0;

    LONG status = RegQueryInfoKeyW(hkey, NULL, NULL, NULL, &subkeys,
                                   NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    return (status == ERROR_SUCCESS) ? (int32_t)subkeys : -1;
}

int32_t jocky_reg_value_count(jocky_reg_handle_t key) {
    if (!key) return -1;

    HKEY hkey = jocky_to_win_key(key);
    DWORD values = 0;

    LONG status = RegQueryInfoKeyW(hkey, NULL, NULL, NULL, NULL,
                                   NULL, NULL, &values, NULL, NULL, NULL, NULL);

    return (status == ERROR_SUCCESS) ? (int32_t)values : -1;
}

int32_t jocky_reg_query_dword(jocky_reg_handle_t key, const char* value_name, uint32_t* out_value) {
    if (!out_value) return -1;

    uint32_t data = 0;
    uint32_t size = sizeof(uint32_t);
    uint32_t type;

    if (jocky_reg_query_value(key, value_name, &type, &data, &size) != 0) {
        return -1;
    }

    if (type != JOCKY_REG_DWORD) return -1;

    *out_value = data;
    return 0;
}

int32_t jocky_reg_set_dword(jocky_reg_handle_t key, const char* value_name, uint32_t value) {
    return jocky_reg_set_value(key, value_name, JOCKY_REG_DWORD, &value, sizeof(uint32_t));
}

int32_t jocky_reg_query_string(jocky_reg_handle_t key, const char* value_name,
                              char* buffer, uint32_t buffer_size) {
    if (!buffer) return -1;

    uint32_t size = buffer_size;
    uint32_t type;

    if (jocky_reg_query_value(key, value_name, &type, buffer, &size) != 0) {
        return -1;
    }

    if (type != JOCKY_REG_SZ && type != JOCKY_REG_EXPAND_SZ) return -1;

    return 0;
}

int32_t jocky_reg_set_string(jocky_reg_handle_t key, const char* value_name, const char* value) {
    if (!value) return -1;

    return jocky_reg_set_value(key, value_name, JOCKY_REG_SZ,
                              value, (uint32_t)(strlen(value) + 1));
}

int32_t jocky_reg_query_binary(jocky_reg_handle_t key, const char* value_name,
                              void* buffer, uint32_t* buffer_size) {
    if (!buffer_size) return -1;

    uint32_t type;

    if (jocky_reg_query_value(key, value_name, &type, buffer, buffer_size) != 0) {
        return -1;
    }

    if (type != JOCKY_REG_BINARY) return -1;

    return 0;
}

int32_t jocky_reg_set_binary(jocky_reg_handle_t key, const char* value_name,
                            const void* data, uint32_t data_size) {
    return jocky_reg_set_value(key, value_name, JOCKY_REG_BINARY, data, data_size);
}

#else  /* Non-Windows: stub implementations */

jocky_reg_handle_t jocky_reg_open(jocky_reg_handle_t hkey, const char* subkey, uint32_t access) {
    return NULL;
}

jocky_reg_handle_t jocky_reg_create(jocky_reg_handle_t hkey, const char* subkey, uint32_t access) {
    return NULL;
}

int32_t jocky_reg_close(jocky_reg_handle_t key) {
    return -1;
}

int32_t jocky_reg_query_value(jocky_reg_handle_t key, const char* value_name,
                             uint32_t* type, void* data, uint32_t* data_size) {
    return -1;
}

int32_t jocky_reg_set_value(jocky_reg_handle_t key, const char* value_name,
                           uint32_t type, const void* data, uint32_t data_size) {
    return -1;
}

int32_t jocky_reg_delete_value(jocky_reg_handle_t key, const char* value_name) {
    return -1;
}

int32_t jocky_reg_delete_key(jocky_reg_handle_t hkey, const char* subkey) {
    return -1;
}

int32_t jocky_reg_enum_key(jocky_reg_handle_t key, uint32_t index,
                          char* name, uint32_t* name_size) {
    return -1;
}

int32_t jocky_reg_enum_value(jocky_reg_handle_t key, uint32_t index,
                            char* name, uint32_t* name_size, uint32_t* type) {
    return -1;
}

int32_t jocky_reg_key_count(jocky_reg_handle_t key) {
    return -1;
}

int32_t jocky_reg_value_count(jocky_reg_handle_t key) {
    return -1;
}

int32_t jocky_reg_query_dword(jocky_reg_handle_t key, const char* value_name, uint32_t* out_value) {
    return -1;
}

int32_t jocky_reg_set_dword(jocky_reg_handle_t key, const char* value_name, uint32_t value) {
    return -1;
}

int32_t jocky_reg_query_string(jocky_reg_handle_t key, const char* value_name,
                              char* buffer, uint32_t buffer_size) {
    return -1;
}

int32_t jocky_reg_set_string(jocky_reg_handle_t key, const char* value_name, const char* value) {
    return -1;
}

int32_t jocky_reg_query_binary(jocky_reg_handle_t key, const char* value_name,
                              void* buffer, uint32_t* buffer_size) {
    return -1;
}

int32_t jocky_reg_set_binary(jocky_reg_handle_t key, const char* value_name,
                            const void* data, uint32_t data_size) {
    return -1;
}

#endif
