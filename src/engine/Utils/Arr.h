//
// Created by vd on 05.05.23.
//

#ifndef ENGINE_ARR_H
#define ENGINE_ARR_H

#include <stdlib.h>
#include <GL/glew.h>

#ifndef BASE_CAPACITY
#define BASE_CAPACITY 10
#endif

typedef enum{
    TYPE_NORMAL,
    TYPE_PTR
} Types;

typedef struct {
    void* data;
    size_t size;
    size_t capacity;
    Types type;
    size_t elementSize;
} Arr;

Arr* makeArray(Types type, size_t elementSize);
void Arr_shrink(Arr* arr);
void Arr_push_GLuint(Arr* arr, GLuint elm);
void Arr_push_GLfloat(Arr* arr, GLfloat elm);
void Arr_delete(Arr* arr);
void Arr_resize(Arr* arr, size_t newSize);


#endif //ENGINE_ARR_H
