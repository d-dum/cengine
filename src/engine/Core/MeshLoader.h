//
// Created by vd on 28.12.22.
//

#ifndef ENGINE_MESHLOADER_H
#define ENGINE_MESHLOADER_H

#include "../Utils/FileUtils.h"

#include <GL/glew.h>

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint cbo;
    GLuint textureID;
    GLuint uv;
    long dataSize;
} Mesh;

Mesh* loadMesh(GLfloat* data, long dataLength);
void addCBO(Mesh* mesh, GLfloat* data, long dataSize);
void loadUV(Mesh* mesh, GLfloat* data, long dataSize);
void deleteMesh(Mesh* mesh);
void loadBMPTexture(Mesh* mesh, BMPImage* image, char freeAfterLoad);

#endif //ENGINE_MESHLOADER_H
