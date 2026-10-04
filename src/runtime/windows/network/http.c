#include <winsock2.h>
#include <wininet.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winhttp.lib")

int64_t jocky_http_get(const char* url, int8_t* out_buf, int64_t max_size) {
    if (!url || !out_buf || max_size <= 0) return -1;

    HINTERNET hInternetSession = InternetOpen("JOCKY", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternetSession) return -1;

    HINTERNET hInternetFile = InternetOpenUrl(hInternetSession, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hInternetFile) {
        InternetCloseHandle(hInternetSession);
        return -1;
    }

    DWORD dwBytesRead = 0;
    int64_t total_read = 0;
    DWORD bufferSize = (max_size < 4096) ? (DWORD)max_size : 4096;
    char* buffer = (char*)malloc(bufferSize);

    if (buffer) {
        while (InternetReadFile(hInternetFile, buffer, bufferSize, &dwBytesRead)) {
            if (dwBytesRead == 0) break;
            if (total_read + dwBytesRead <= (uint64_t)max_size) {
                memcpy(out_buf + total_read, buffer, dwBytesRead);
            }
            total_read += dwBytesRead;
        }
        free(buffer);
    }

    InternetCloseHandle(hInternetFile);
    InternetCloseHandle(hInternetSession);
    return total_read;
}

unsigned char jocky_http_post(const char* url, unsigned char* data, uint32_t data_size, const char* headers) {
    HINTERNET hSession = InternetOpen("JOCKY", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hSession) return 0;

    URL_COMPONENTS urlComp = {0};
    char szHostName[256] = {0};
    char szPath[256] = {0};

    urlComp.dwStructSize = sizeof(urlComp);
    urlComp.lpszHostName = szHostName;
    urlComp.dwHostNameLength = sizeof(szHostName);
    urlComp.lpszUrlPath = szPath;
    urlComp.dwUrlPathLength = sizeof(szPath);

    if (!InternetCrackUrl(url, 0, 0, &urlComp)) {
        InternetCloseHandle(hSession);
        return 0;
    }

    HINTERNET hConnect = InternetConnect(hSession, szHostName, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        InternetCloseHandle(hSession);
        return 0;
    }

    HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", szPath, NULL, NULL, NULL, 0, 0);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        return 0;
    }

    unsigned char result = (unsigned char)HttpSendRequest(hRequest, headers, (DWORD)strlen(headers ? headers : ""), data, data_size);

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);
    return result;
}

bool jocky_download_file(const char* url, const char* dest_path) {
    if (!url || !dest_path) return false;

    HINTERNET hSession = InternetOpen("JOCKY", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hSession) return false;

    HINTERNET hFile = InternetOpenUrl(hSession, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hFile) {
        InternetCloseHandle(hSession);
        return false;
    }

    FILE* fpDest = fopen(dest_path, "wb");
    if (!fpDest) {
        InternetCloseHandle(hFile);
        InternetCloseHandle(hSession);
        return false;
    }

    char buffer[4096];
    DWORD dwBytesRead = 0;

    while (InternetReadFile(hFile, buffer, sizeof(buffer), &dwBytesRead)) {
        if (dwBytesRead == 0) break;
        fwrite(buffer, 1, dwBytesRead, fpDest);
    }

    fclose(fpDest);
    InternetCloseHandle(hFile);
    InternetCloseHandle(hSession);
    return true;
}
