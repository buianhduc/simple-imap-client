//
// Created by Đức Bùi on 3/5/24.
//

#ifndef _EMAIL_HANDLE_H_
#define _EMAIL_HANDLE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include "utils.h"
#include "communication.h"

void retrieve_message(int connfd, int num_message);

#endif //_EMAIL_HANDLE_H_
