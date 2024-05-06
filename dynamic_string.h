//
// Created by Đức Bùi on 4/5/24.
//

#ifndef _DYNAMIC_STRING_H_
#define _DYNAMIC_STRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>

struct dynamic_string{
    char* str;
    size_t len;
    size_t size;
};

typedef struct dynamic_string string;

string* create_string(size_t initialSize);
string* create_string_from_char(char* src);
string* create_string_from_string(const string* s);
string* append(string* dst, char* src);
void free_string(string* str);
#endif //_DYNAMIC_STRING_H_
