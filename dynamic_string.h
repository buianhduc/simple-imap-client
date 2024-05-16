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
/**
 * @fn create_string
 * @param initalSize
 * @return allocate dynamic string and return its pointer
*/
string* create_string(size_t initialSize);
/**
 * @fn create_string_from_char
 * @param src
 * @return allocate dynamic string, copy the src string and return its pointer
*/
string* create_string_from_char(char* src);
/**
 * @fn create_string_from_string
 * @param src
 * @return allocate and copy from src string
*/
string* create_string_from_string(const string* src);
/**
 * @fn append
 * @param dst
 * @param src
 * @return dst after appended
*/
string* append(string* dst, char* src);
/**
 * @fn free_string
 * free content of src
 * @param str
*/
void free_string(string* str);
#endif //_DYNAMIC_STRING_H_
