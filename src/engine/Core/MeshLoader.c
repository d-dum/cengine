//
// Created by vd on 28.12.22.
//

#include "MeshLoader.h"

#include <GL/glew.h>
#include <stdlib.h>
#include "../../../lib/fast_obj/fast_obj.h"

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
    mesh->cbo = -1;
    mesh->textureID = -1;
    mesh->uv = -1;
    mesh->ebo = -1;
    return mesh;
}

Mesh* loadMeshWithIndices(GLfloat* data, long dataSize, GLuint* indices, long indicesSize){
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Mesh* mesh = (Mesh*) calloc(1, sizeof(Mesh));
    mesh->vao = vao;
    mesh->vbo = vbo;
    mesh->ebo = ebo;
    mesh->dataSize = indicesSize;

    mesh->cbo = -1;
    mesh->uv = -1;
    mesh->textureID = -1;
    return mesh;
}

Mesh* loadFromOBJ(char* path, char hasUvs){
    fastObjMesh* mesh = fast_obj_read(path);

    GLfloat* sortedUvs = (GLfloat*) calloc(mesh->index_count * 2, sizeof(GLfloat));
    GLuint* indices = (GLuint*) calloc(mesh->index_count, sizeof(GLuint));

    for(unsigned int i = 0; i < mesh->face_count; i++){
        unsigned int v0 = mesh->indices[i * 3 + 0].p;
        unsigned int v1 = mesh->indices[i * 3 + 1].p;
        unsigned int v2 = mesh->indices[i * 3 + 2].p;

        unsigned int uv0 = mesh->indices[i * 3 + 0].t;
        unsigned int uv1 = mesh->indices[i * 3 + 1].t;
        unsigned int uv2 = mesh->indices[i * 3 + 2].t;

        sortedUvs[i * 6 + 0] = mesh->texcoords[uv0 * 2 + 0];
        sortedUvs[i * 6 + 1] = mesh->texcoords[uv0 * 2 + 1];
        sortedUvs[i * 6 + 2] = mesh->texcoords[uv1 * 2 + 0];
        sortedUvs[i * 6 + 3] = mesh->texcoords[uv1 * 2 + 1];
        sortedUvs[i * 6 + 4] = mesh->texcoords[uv2 * 2 + 0];
        sortedUvs[i * 6 + 5] = mesh->texcoords[uv2 * 2 + 1];

        indices[i*3+0] = v0;
        indices[i*3+1] = v1;
        indices[i*3+2] = v2;
    }

    Mesh* ret = loadMeshWithIndices(mesh->positions, mesh->position_count * sizeof(float), indices, mesh->index_count * sizeof(GLuint));

    if(hasUvs){
        loadUV(ret, sortedUvs, mesh->index_count*2*sizeof(GLfloat));
    }

    fast_obj_destroy(mesh);
    free(sortedUvs);
    free(indices);

    return ret;
}

void addCBO(Mesh* mesh, GLfloat* data, long dataSize){
    glBindVertexArray(mesh->vao);
    GLuint cbo;
    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    mesh->cbo = cbo;
}

void loadUV(Mesh* mesh, GLfloat* data, long dataSize){
    glBindVertexArray(mesh->vao);
    GLuint uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    mesh->uv = uvBuffer;
}

void loadTextureGeneric(Mesh* mesh, unsigned int width, unsigned int height, unsigned char* pixels){
    glBindVertexArray(mesh->vao);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei) width, (GLsizei) height,
                 0, GL_BGR, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    mesh->textureID = textureID;
}

void loadBMPTexture(Mesh* mesh, BMPImage* image, char freeAfterLoad){
    loadTextureGeneric(mesh, image->width, image->height, image->data);
    if(freeAfterLoad){
        freeBMP(image);
    }
}

void loadPNGTexture(Mesh* mesh, PNGImage* image, char freeAfterLoad){
    loadTextureGeneric(mesh, image->width, image->height, image->data);
    if(freeAfterLoad){
        freePNG(image);
    }
}

void deleteMesh(Mesh* mesh){
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->cbo);
    glDeleteVertexArrays(1, &mesh->vao);
    free(mesh);
}