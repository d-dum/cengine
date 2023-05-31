#ifndef LABEL_H
#define LABEL_H

#include <GL/glew.h>

typedef struct Label{
    float height;
    GLuint* textures;
    GLuint list_base;
} Label;

#endif