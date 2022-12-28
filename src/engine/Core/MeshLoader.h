//
// Created by vd on 28.12.22.
//

#ifndef ENGINE_MESHLOADER_H
#define ENGINE_MESHLOADER_H

#include <GL/glew.h>

typedef struct {
    GLuint vao;
    GLuint vbo;
    long dataSize;
} Mesh;

Mesh* loadMesh(GLfloat* data, long dataLength);
void deleteMesh(Mesh* mesh);

#endif //ENGINE_MESHLOADER_H
