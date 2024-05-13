//
// Created by Đức Bùi on 3/5/24.
//

#include "email_handle.h"
#define LINE_LIMIT 998
#define CRLF "\r\n"
#define SPACE 0x20
#define TAB 0x09
#define MIME_HEADER_START "MIME-Version: 1.0\r\nContent-Type: multipart/alternative;"
#define DEFAULT_BOUNDARY_LENGTH 70

char* strdup(const char *s);
char* strcasestr(char*, char*);
int strncasecmp(const char*, const char*, unsigned long);

struct mime_s{
    char* plain_message;
    char* encoding_type;
    char* content_type;
};

char* get_mime_plain(char* msg){
    char* content = strstr(msg, "\r\n\r\n");
    char* contentEnd = strstr(msg, "\r\n\0");
    content += 4;

    *contentEnd = '\0';
    content = strdup(content);
    return content;
}

string *get_mime_section(char *content){

    if (content == NULL) return NULL;
    // Find MIME Header
    char* mimeHeader = strcasestr(content, "\r\nMIME-Version:");
    if (mimeHeader == NULL) return NULL;

    char* contentType = strcasestr(mimeHeader, "\r\nContent-type:");
    if (contentType == NULL) return NULL;

    // Find Boundary value
    char* boundaryValuePtr = strcasestr(contentType, "boundary=");
    if (boundaryValuePtr == NULL) return NULL;
    boundaryValuePtr += strlen("boundary=");
    char *tmp = boundaryValuePtr;
    if (*tmp == '"') {
        tmp++;
        boundaryValuePtr ++;
        for (; *tmp != '"' && (tmp - boundaryValuePtr) <= DEFAULT_BOUNDARY_LENGTH; tmp++){
//            if (*tmp == '\\') tmp++;
        }
    } else
        for (; *tmp != '\r' && *(tmp+1) != '\n'; tmp++);
    char* boundaryValue = calloc(DEFAULT_BOUNDARY_LENGTH, sizeof(char));
    if (boundaryValue == NULL){
        free(content);
        return NULL;
    }
    boundaryValue = strncpy(boundaryValue, boundaryValuePtr, (tmp - boundaryValuePtr));
    boundaryValue[tmp - boundaryValuePtr] = '\0';


    // Extract content within boundary
    char* boundaryStart = NULL, *boundaryEnd = NULL;
    (void) asprintf(&boundaryStart, "--%s\r\n", boundaryValue);
    (void) asprintf(&boundaryEnd, "--%s--\r\n", boundaryValue);
    if (boundaryStart == NULL ||boundaryEnd == NULL){
        exit(E_OTHER);
    }

    char* sectionStart = strcasestr(content, boundaryStart);
    char* sectionEnd = strcasestr(content, boundaryEnd);
    if (sectionEnd == NULL || sectionStart == NULL) exit (E_PARSE);

    if (sectionEnd - content + strlen(boundaryEnd) > strlen(content)) {
        exit(E_PARSE);
    }

    sectionEnd += strlen(boundaryEnd);

    string* mimeSection = create_string(sectionEnd - sectionStart + 2);
    mimeSection->str = strncpy(mimeSection->str, sectionStart, sectionEnd - sectionStart + 1);
    mimeSection->len = (sectionEnd - sectionStart);
    mimeSection->str[mimeSection->len] = '\0';

    free(boundaryStart);
    free(boundaryEnd);
    return mimeSection;
}

