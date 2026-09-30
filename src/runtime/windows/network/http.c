#include <winsock2.h>
#include <wininet.h>
#include <string.h>
#include <stdlib.h>

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "ws2_32.lib")

i64 jocky_http_get(const char* url, char* out_buf, i64 max_size) {
    HINTERNET hInternetSession = InternetOpen("JOCKY", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternetSession) return 0;

    HINTERNET hInternetFile = InternetOpenUrl(hInternetSession, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hInternetFile) {
        InternetCloseHandle(hInternetSession);
        return 0;
    }

    DWORD dwBytesRead = 0;
    i64 total_read = 0;
    DWORD bufferSize = (max_size < 4096) ? max_size : 4096;
    char* buffer = (char*)malloc(bufferSize);

    if (buffer) {
        while (InternetReadFile(hInternetFile, buffer, bufferSize, &dwBytesRead)) {
            if (dwBytesRead == 0) break;
            if (out_buf && total_read + dwBytesRead <= max_size) {
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

bool jocky_http_post(const char* url, i8* data, i32 data_size, const char* headers) {
    HINTERNET hSession = InternetOpen("JOCKY", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hSession) return false;

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
        return false;
    }

    HINTERNET hConnect = InternetConnect(hSession, szHostName, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (!hConnect) {
        InternetCloseHandle(hSession);
        return false;
    }

    HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", szPath, NULL, NULL, NULL, 0, 0);
    if (!hRequest) {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        return false;
    }

    bool result = HttpSendRequest(hRequest, headers, strlen(headers ? headers : ""), data, data_size);

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);
    return result;
}

bool jocky_download_file(const char* url, const char* dest_path) {
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
    bool bRead = true;

    while (bRead && InternetReadFile(hFile, buffer, sizeof(buffer), &dwBytesRead)) {
        if (dwBytesRead == 0) break;
        fwrite(buffer, 1, dwBytesRead, fpDest);
    }

    fclose(fpDest);
    InternetCloseHandle(hFile);
    InternetCloseHandle(hSession);
    return true;
}
