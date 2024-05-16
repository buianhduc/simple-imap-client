//
// Created by Đức Bùi on 1/5/24.
//

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *strremove(char *str, const char *sub) ;
char *remove_CRLF(char* str, size_t* msgLen);
size_t get_strlen(const char* str);
#endif //_UTILS_H_
