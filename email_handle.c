//
// Created by Đức Bùi on 3/5/24.
//

#include "email_handle.h"
#define LINE_LIMIT 998
#define CRLF "\r\n"
#define SPACE 0x20
#define TAB 0x09
#define MIME_HEADER_START "MIME-Version: 1.0\r\nContent-Type: multipart/alternative;"

char* strdup(const char *s);

char* get_mime_boundary_start(const char* value){
}


struct field* parse_header(string headerContent){
    // struct field* fields = calloc(2048, sizeof(struct field));
    // char *header = headerContent.str;
    // char *rest = header;
    // char* line = strtok_r(line, CRLF, &rest);
    // struct field toBeFeed = {NULL, NULL};
    // int size = 0;
    // int fieldInit = 0;
    // while (line != NULL){
    //     // If no field been init
    //     if (!fieldInit){
    //         // Find Colon (where the field's name ends)
    //         char* colonPos = strchr(line,":");
    //         if (colonPos == NULL) {
    //             fprintf(stderr, "Error parsing email's field");
    //             exit(E_PARSE);
    //         }
    //         toBeFeed.
    //         toBeFeed.content = "";
    //         strncpy(toBeFeed.name, line, colonPos - line);
    //         fieldInit = 1;
    //     } else {
    //         // Finished reading field's content
    //         if (line[0] != SPACE && line[0] != TAB) {
    //             fieldInit = 0;
    //             fields[size] = toBeFeed;
    //             continue;
    //         }
    //         toBeFeed.content = realloc(sizeof(line)+1);
    //         strncpy(toBeFeed.content, line, sizeof(line));
    //     }
    // }
}

string* get_mime_content(char* message, const char* boundaryValue){
    
}
char* get_mime_plain(const char* mimeStart, const char* mimeEnd, const char* boundaryStart, const char* boundaryEnd){
    // printf("%s", i);
    return NULL;
}

char* get_mime(const char* msg){
    char* ptr = strstr(msg, MIME_HEADER_START);
    // Cannot find
    if (ptr == NULL) {
        return NULL;
    }
    // Find MIME first header
    char* boundary = strstr(ptr, "boundary=");
    if (boundary == NULL) return NULL;

    char* endOfBoundary = strstr(boundary, CRLF);
    if (endOfBoundary  == NULL) return NULL;
    
    // Get MIME boundary value
    int boundaryValueLength = (endOfBoundary - boundary - strlen("boundary=")) + 1;
    char* boundaryValue = calloc(boundaryValueLength + 2, sizeof (char));
    strncpy(boundaryValue, boundary+strlen("boundary="), boundaryValueLength);
    boundaryValue[boundaryValueLength-1] = '\0';

    // Locate the boundary of the MIME field
    char* boundaryStart = NULL;
    asprintf(&boundaryStart, "\r\n--%s", boundaryValue);

    char* boundaryEnd = NULL;
    asprintf(&boundaryEnd, "\r\n--%s--", boundaryValue);

    char* mimeStart = strstr(boundary, boundaryStart);
    if (mimeStart == NULL) {
        free(boundaryStart);
        free(boundaryEnd);
        return NULL;   
    }

    char* mimeEnd = strstr(boundary, boundaryEnd);
    if (mimeEnd == NULL){
        free(boundaryStart);
        free(boundaryEnd);
        return NULL;
    }
    
    char* result = get_mime_plain(mimeStart, mimeEnd, boundaryStart, boundaryEnd);
    
    return NULL;
}

