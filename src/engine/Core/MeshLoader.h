//
// Created by vd on 28.12.22.
//

#ifndef ENGINE_MESHLOADER_H
#define ENGINE_MESHLOADER_H

#include "../Utils/FileUtils.h"

#include <GL/glew.h>

typedef struct Mesh {
    GLuint vao;
    GLuint vbo;
    GLuint cbo;
    GLuint textureID;
    GLuint uv;
    GLuint nbo;
    long dataSize;
    GLuint ebo;
    float shineDamper;
    float reflectivity;
} Mesh;

Mesh* loadMesh(GLfloat* data, long dataLength);
void addCBO(Mesh* mesh, GLfloat* data, long dataSize);
void loadUV(Mesh* mesh, GLfloat* data, long dataSize);
void deleteMesh(Mesh* mesh);
void loadBMPTexture(Mesh* mesh, BMPImage* image, char freeAfterLoad);
void loadTextureGeneric(Mesh* mesh, unsigned int width, unsigned int height, unsigned char* pixels);
void loadPNGTexture(Mesh* mesh, PNGImage* image, char freeAfterLoad);
Mesh* loadMeshWithIndices(GLfloat* data, long dataSize, GLuint* indices, long indicesSize);
Mesh* loadFromOBJ(char* path, char hasUvs);
Mesh* loadGLTF(char* path);
void loadAnyTexture(Mesh* mesh, char* path);
void addNormals(Mesh* mesh, GLfloat* data, long dataSize);

#endif //ENGINE_MESHLOADER_H
