//
// Created by Đức Bùi on 1/5/24.
//

#include "utils.h"

char *strremove(char *str, const char *sub) {
    char *p, *q, *r;
    if (*sub && (q = r = strstr(str, sub)) != NULL) {
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL) {
            while (p < r)
                *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0')
            continue;
    }
    return str;
}
// referenced from https://stackoverflow.com/questions/47116974/remove-a-substring-from-a-string-in-c
char *remove_CRLF(char *str, size_t* msgLen) {
    if (strpbrk(str, "\0") != NULL) return str;
    str[(*msgLen)] = '\0';
    strremove(str, "\r");

    return str;
}

size_t get_strlen(const char *str){
    int length = 0;
    while(str[length++] != '\0');
    return length-1;
}