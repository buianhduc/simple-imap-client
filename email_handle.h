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
#include "utils.h"
#include "communication.h"
#include "dynamic_string.h"

// ===== Email conventions =====
#define LINE_FOLDING "\r\n "
// ===== End of email conventions =====

// ==== structs ====
struct field{
    string name;
    string content;
};
// ==== end of structs ====

struct field* parse_header(string headerContent); 
char* get_mime(const char* msg);

#endif //_EMAIL_HANDLE_H_
