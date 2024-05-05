//
// Created by Đức Bùi on 4/5/24.
//

#include "dynamic_string.h"

string* create_string(size_t initialSize){
    string* new = malloc(sizeof(string));
    assert(new);
    new->size = initialSize + 1;
    new->len  = 0;
    new->str = calloc(initialSize, sizeof(char));
    return new;
}
string* create_string_from_char(char* src){
    string* new = create_string(strlen(src));
    (void) strcat(new->str, src);
    new->len = strlen(src);
    return new;
}
string* append(string* dst, char* src){
    ssize_t len = strlen(src);
    if (dst->size < len + dst->len){
        char* tmp = realloc(dst->str, dst->size + 2*len + 5);
        assert(tmp != NULL);
        dst->str = tmp;
        dst->size += (2*len + 5);
    }
    dst->len += len;
    (void) strncat(dst->str, src, len);
    
    return dst;
}
void free_string(string* str){
    free(str->str);
    free(str);
}