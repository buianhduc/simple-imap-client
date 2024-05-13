//
// Created by Đức Bùi on 1/5/24.
//
#define _POSIX_C_SOURCE 200112L
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_
#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ERROR_CODE.h"
#include "IMAP_RESPONSE.h"
#include "dynamic_string.h"
#include "utils.h"

struct addrinfo;

int asprintf(char **ret, const char *format, ...);
// ===== Base operation for communicating with the server ====
void handle_error(int errCode);
int create_connection(char* emailServer, char* port, struct addrinfo **res);
void check_response(int connfd);
string *recv_from_server(int connfd, char* tag);
ssize_t send_to_server(int connfd, char* command, size_t nbytes);


// ===== Routines for client =====
int login_to_server(int connfd, char *username, char *password);
int select_folder(int connfd, const char *folderDirectory);
string* retrieve_email(int connfd, unsigned long num_message);
void retrieve_message(int connfd, int num_message);
#endif //_COMMUNICATION_H_
