//
// Created by Đức Bùi on 3/5/24.
//
#define _POSIX_C_SOURCE 200112L
#ifndef _EMAIL_HANDLE_H_
#define _EMAIL_HANDLE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include "utils.h"
#include "communication.h"
#include "dynamic_string.h"

// ===== Email conventions =====
#define LINE_FOLDING "\r\n "
// ===== End of email conventions =====

// ==== structs ====
struct field{
    string* name;
    string* content;
};
struct mime{
    string* contentTransferEncoding, contentType;
    string* content;
};
// ==== end of structs ====

void parse_header(int connfd, int message_num);
string* parse_field(string* buff, char* field);
string *get_mime_section(char *content);
void list_email(int connfd);
string* parse_subject(string* buff, int content_start);

#endif //_EMAIL_HANDLE_H_
