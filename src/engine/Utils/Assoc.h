#ifndef ASSOC_H
#define ASSOC_H

#include <stdlib.h>
#include <string.h>

#define ASSOCIATIVE_ARRAY(type, name) \
    typedef struct List_##name {\
        char key[50]; \
        type value; \
        struct List_##name* next; \
    } List_##name;


#define CREATE_NODE(type, name) \
    static List_##name* createNode_##name(const char* key, type value){\
        List_##name* newNode = malloc(sizeof(List_##name)); \
        strncpy(newNode->key, key, sizeof(newNode->key) - 1); \
        newNode->value = value; \
        newNode->next = NULL; \
        return newNode; \
    }

#define FIND_NODE(type, name) \
    List_##name* findNode_##name(const char* key, List_##name* root){\
        List_##name* rt = root; \
        while(rt != NULL){ \
            if(strcmp(key, rt->key) == 0) return rt; \
            rt = rt->next; \
        } \
        return NULL; \
    }

#define NODE_APPEND(type, name) \
    void appendNode_##name(List_##name* root, List_##name* node){\
        List_##name* rt = root; \
        while(rt->next != NULL){ \
            rt = rt->next; \
        } \
        rt->next = node; \
    }

#endif