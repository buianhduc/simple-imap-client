//
// Created by Đức Bùi on 2/5/24.
//

#ifndef _DYNAMICSTRING_H_
#define _DYNAMICSTRING_H_

// Default config
#define DEFAULT_MEM 1024

struct dynamicString {
    int length;
    int availableSize;
    char* content;
};

struct dynamicString* create_str();


#endif //_DYNAMICSTRING_H_
