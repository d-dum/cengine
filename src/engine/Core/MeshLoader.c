//
// Created by vd on 28.12.22.
//

#include "MeshLoader.h"
#include <GL/glew.h>
#include <stdlib.h>

Mesh* loadMesh(GLfloat* data, long dataSize){
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Mesh* mesh = (Mesh*) malloc(sizeof(Mesh));
    mesh->dataSize = dataSize;
    mesh->vao = vao;
    mesh->vbo = vbo;
    return mesh;
}

void deleteMesh(Mesh* mesh){
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteVertexArrays(1, &mesh->vao);
    free(mesh);
}