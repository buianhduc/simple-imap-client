//
// Created by Đức Bùi on 1/5/24.
//

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <getopt.h>
#include <netdb.h>
#include "ERROR_CODE.h"

void handle_error(int errCode);

int create_connection(char* emailServer, char* port, struct addrinfo **res);
//char* read_message(int connfd, char* )
void check_response(int connfd);

#endif //_COMMUNICATION_H_
