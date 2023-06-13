#ifndef LABEL_H
#define LABEL_H

#include <GL/glew.h>

typedef struct Label{
    float height;
    GLuint* textures;
    GLuint list_base;
} Label;

typedef struct Character {
    unsigned int textureID;
    int size_x;
    int size_y;
    int bearing_x;
    int bearing_y;
    unsigned int advance;
} Character;

Character** loadFont(char* path, int pixelSize);
void deleteCharSet(Character** charSet);

#endif