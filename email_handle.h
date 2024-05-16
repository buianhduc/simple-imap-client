//
// File where email-related functions
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

/**
 * @fn: parse_header 
 * parse header of specified email
 * @param connfd: connection file descriptor
 * @param message_num: the message number wished to get header, -1 gets the most recent
*/
void parse_header(int connfd, long long message_num);
/**
 * @fn: parse_field 
 * parse field for email header
 * @param connfd: connection file descriptor
 * @param message_num: the message number wished to get header, -1 gets the most recent
*/
string* parse_field(string* buff, char* field);
/**
 * @fn: get_mime_section:
 * return first text/plain mime section
 * @param content: raw email
 * @return first text/plain mime section
*/
string *get_mime_section(char *content);
void list_email(int connfd);


#endif //_EMAIL_HANDLE_H_
