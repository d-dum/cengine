//
// Created by vd on 05.05.23.
//

#include "Arr.h"

Arr* makeArray(Types type, size_t elementSize){
    Arr* arr = (Arr*) malloc(sizeof(Arr));
    arr->type = type;
    arr->capacity = BASE_CAPACITY;
    arr->size = 0;
    arr->elementSize = elementSize;
    arr->data = malloc(elementSize * BASE_CAPACITY);
    return arr;
}


void Arr_shrink(Arr* arr){
    void* buf = realloc(arr->data, arr->elementSize * arr->size);
    if(buf != NULL)
        arr->data = buf;
}

void Arr_push_GLuint(Arr* arr, GLuint elm){
    if(arr->size + 1 < arr->capacity){
        ++arr->size;
        GLuint* data = (GLuint*)arr->data;
        data[arr->size - 1] = elm;
        arr->data = data;
        return;
    }

    arr->capacity += BASE_CAPACITY;
    GLuint* buf = realloc(arr->data, arr->capacity * arr->elementSize);
    if(buf != NULL){
        buf[arr->size++] = elm;
    }
}

void Arr_push_GLfloat(Arr* arr, GLfloat elm){
    if(arr->size + 1 < arr->capacity){
        ++arr->size;
        GLfloat* data = (GLfloat*)arr->data;
        data[arr->size - 1] = elm;
        arr->data = data;
        return;
    }

    arr->capacity += BASE_CAPACITY;
    GLfloat* buf = realloc(arr->data, arr->capacity * arr->elementSize);
    if(buf != NULL){
        buf[arr->size++] = elm;
    }
}

void Arr_delete(Arr* arr){
    free(arr->data);
    free(arr);
}

void Arr_resize(Arr* arr, size_t newSize){
    if(newSize < arr->capacity){
        return;
    }

    arr->capacity = newSize;
    void* buf = realloc(arr->data, arr->elementSize * newSize);
    if(buf != NULL)
        arr->data = buf;
}