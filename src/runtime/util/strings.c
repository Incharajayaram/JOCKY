#include <string.h>
#include <stdlib.h>

char* jocky_str_concat(const char* str1, const char* str2) {
    if (!str1) str1 = "";
    if (!str2) str2 = "";

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    char* result = (char*)malloc(len1 + len2 + 1);

    if (!result) return strdup(str1);

    strcpy(result, str1);
    strcat(result, str2);

    return result;
}
