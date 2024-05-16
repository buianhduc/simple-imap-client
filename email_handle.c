//
// Created by Đức Bùi on 3/5/24.
//

#include "email_handle.h"
#include "communication.h"
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

typedef struct emailHeader{
    char* from;
    char* to;
    char* date;
    char* subject;
}emailHeader_t;

string* get_mime_plain(string* msg, char* boundaryStart, char* boundaryEnd){
    string* result = NULL;
    char* contentType = NULL, *encoding = NULL;
    // Get the end of the current section
    char* sectionStart = msg->str;
    char* sectionEnd = strcasestr(sectionStart + strlen(boundaryStart), boundaryStart);
    char* nextSectionEnd = sectionEnd;
    if (sectionEnd == NULL ){
        sectionEnd = strcasestr(sectionStart, boundaryEnd);
        if (sectionEnd == NULL) exit(E_PARSE);
    }
    for (; sectionStart != sectionEnd; sectionEnd = nextSectionEnd){

        nextSectionEnd = strcasestr(sectionEnd + strlen(boundaryStart), boundaryStart);
        if (nextSectionEnd == NULL ){
            nextSectionEnd = strcasestr(sectionEnd, boundaryEnd);
            if (nextSectionEnd == NULL) exit(E_PARSE);
        }
        contentType = strcasestr(msg->str, "\r\nContent-Type:");
        if (contentType != NULL) {
            contentType += strlen("\r\nContent-Type: ");
            if (strncasecmp(contentType, "text/plain", strlen("text/plain")) != 0)
                continue;
            encoding = strcasestr(msg->str, "\r\nContent-Transfer-Encoding:");
            char* start = contentType > encoding ? contentType : encoding;
            start += 2;
            for (; *start != '\r' && *(start+1) != '\n'; start++);
            result = create_string(sectionEnd - start - strlen("\r\n\r\n") +1);
            result->str = strncpy(result->str, start+strlen("\r\n\r\n"), sectionEnd - start - strlen("\r\n\r\n") + 1);
            result->len = sectionEnd - start - strlen("\r\n\r\n");
            result->str[result->len] = '\0';
            break;
        }
        sectionStart = sectionEnd;
    }
    return result;
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
        for (; *tmp != '"' && (tmp - boundaryValuePtr) <= DEFAULT_BOUNDARY_LENGTH; tmp++);
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
    (void) asprintf(&boundaryStart, "\r\n--%s\r\n", boundaryValue);
    (void) asprintf(&boundaryEnd, "\r\n--%s--\r\n", boundaryValue);
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
    string* result = get_mime_plain(mimeSection, boundaryStart, boundaryEnd);

    free(boundaryStart);
    free(boundaryEnd);
    free(boundaryValue);
    free_string(mimeSection);
    return result;
}

void parse_header(int connfd, long long int message_num){

    char* tag = get_imap_tag();
    char* fields[] = {"From", "To", "Date", "Subject"};
    for(int i = 0; i < 4; i++){
        char* command = NULL;
        if(message_num == -1){
            asprintf(&command, "%s FETCH * BODY.PEEK[HEADER.FIELDS (%s)]\r\n", tag, fields[i]);
        }else{
            asprintf(&command, "%s FETCH %d BODY.PEEK[HEADER.FIELDS (%s)]\r\n", tag, message_num, fields[i]);
        }
        if(send_to_server(connfd, command, get_strlen(command)) > 0){
            string* buff = recv_from_server(connfd, tag);
            string* result = (parse_field(buff, fields[i]));
            if(strstr(buff->str, IMAP_OK) != NULL){
                printf("%s:%s\n", fields[i], result->str);
            }
            free_string(result);
            free_string(buff);
        }
        free(command);
    }

}

string* parse_field(string* buff, char* field) {
    string* parsed_field;
    size_t field_end = 0;
    for (; field_end < buff->len && buff->str[field_end] != ':'; field_end++);
    int content_start = field_end + 1;
    int content_end = content_start;

    if (strcmp(field, "Subject") != 0) {
        while (content_end < buff->len && buff->str[content_end] != '\r') {
            content_end++;
        }
        parsed_field = create_string_from_char(buff->str + content_start);
        parsed_field->len = content_end - content_start;
        parsed_field->str[parsed_field->len] = '\0';
    } else {
        parsed_field = create_string(2048);
        int unfolded_len = 0;
        for (int i = content_start; i < buff->len; i++) {
            if ((buff->str[i] == '\r') && (i + 2 < buff->len) && (buff->str[i + 1] == '\n') && (buff->str[i + 2] == ' ')) {
                i += 1;
                continue;
            }else if(buff->str[i] == '\r') {
                break;
            }
            parsed_field->str[unfolded_len++] = buff->str[i];
        }
        parsed_field->str[unfolded_len] = '\0';
        parsed_field->len = unfolded_len;
    }
    return parsed_field;
}
