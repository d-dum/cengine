//
// Created by vd on 28.12.22.
//

#include "MeshLoader.h"

#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../../lib/fast_obj/fast_obj.h"

#define CGLTF_IMPLEMENTATION
#include "../../../lib/cgltf/cgltf.h"

#include "../../../lib/SOIL2/src/SOIL2/SOIL2.h"

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
    mesh->nbo = -1;
    mesh->shineDamper = 1.f;
    mesh->reflectivity = 0.f;
    return mesh;
}

Mesh* loadFromOBJ(char* path, char hasUvs){
    fastObjMesh* mesh = fast_obj_read(path);

    GLfloat* sortedUvs = (GLfloat*) calloc(mesh->index_count * 2, sizeof(GLfloat));
    GLfloat* sortedNormals = (GLfloat*) calloc(mesh->position_count * 3, sizeof(GLfloat));
    GLuint* indices;

    unsigned int prev0 = 0, prev1 = 0;

    char first = 1;

    unsigned int inx = 0;

    for(unsigned int i = 0; i < mesh->face_count; i++){
        for(unsigned int j = 0; j < 3; j++){
            unsigned int vertexIndex = mesh->indices[i*3 + j].p;
            unsigned int uvIndex = mesh->indices[i*3 + j].t;

            float uv1 = mesh->texcoords[uvIndex*2];
            float uv2 = mesh->texcoords[uvIndex*2 + 1];

            sortedUvs[vertexIndex*2] = uv1;
            sortedUvs[vertexIndex*2 + 1] = uv2;
        }
    }

    for(unsigned int i = 0; i < mesh->face_count; i++){
        for(unsigned int j = 0; j < 3; j++){
            unsigned int vertexIndex = mesh->indices[i * 3 + j].p;
            unsigned int normalIndex = mesh->indices[i * 3 + j].n;

            float normal1 = mesh->normals[normalIndex * 3];
            float normal2 = mesh->normals[normalIndex * 3 + 1];
            float normal3 = mesh->normals[normalIndex * 3 + 2];

            sortedNormals[vertexIndex * 3] = normal1;
            sortedNormals[vertexIndex * 3 + 1] = normal2;
            sortedNormals[vertexIndex * 3 + 2] = normal3;
        }
    }

    printf("Face count: %d\n", mesh->face_count);
    printf("Index count: %d\n", mesh->index_count);

    long indicesSize = 0;

    if(mesh->index_count / mesh->face_count == 3){
        indices = (GLuint*) calloc(mesh->index_count, sizeof(GLuint));
        indicesSize = mesh->index_count * sizeof(GLuint);
        for(size_t i = 0; i < mesh->index_count; i++){
            indices[i] = mesh->indices[i].p;
        }
    }else{
        indices = (GLuint*) calloc(mesh->face_count * 6, sizeof(GLuint));
        GLuint* buf = calloc(4, sizeof(GLuint));
        indicesSize = mesh->face_count * 6 * sizeof(GLuint);
        size_t inx = 0;
        for(size_t i = 0; i < mesh->face_count; i++){
            for(size_t j = 0; j < 4; j++){
                buf[j] = mesh->indices[i*4+j].p;
            }
            indices[inx++] = buf[0];
            indices[inx++] = buf[1];
            indices[inx++] = buf[2];

            indices[inx++] = buf[0];
            indices[inx++] = buf[2];
            indices[inx++] = buf[3];
        }
        free(buf);
    }


    Mesh* ret = loadMeshWithIndices(mesh->positions, (mesh->position_count) * sizeof(float) * 3, indices, indicesSize);

    printf("Indices size: %ld\n", ret->dataSize / sizeof(GLuint));
    printf("Position count: %d\n", mesh->position_count);


    if(hasUvs){
        loadUV(ret, sortedUvs, mesh->position_count*2*sizeof(GLfloat));
    }

    addNormals(ret, sortedNormals, mesh->position_count * 3 * sizeof(GLfloat));

    fast_obj_destroy(mesh);
    free(sortedUvs);
    free(indices);
    free(sortedNormals);

    return ret;
}

void addNormals(Mesh* mesh, GLfloat* data, long dataSize){
    glBindVertexArray(mesh->vao);
    GLuint nbo;
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mesh->nbo = nbo;
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height,
                 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);
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

void loadAnyTexture(Mesh* mesh, char* path){
    GLuint tex_2d = SOIL_load_OGL_texture(
            path,
            SOIL_LOAD_AUTO,
            SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y
            );
    mesh->textureID = tex_2d;
}

void deleteMesh(Mesh* mesh){
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    if(mesh->ebo != -1)
        glDeleteBuffers(1, &mesh->ebo);
    free(mesh);
}

Mesh* loadGLTF(char* path){
    cgltf_options options = {0};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, path, &data);
    if(result != cgltf_result_success)
        return NULL;

    result = cgltf_load_buffers(&options, data, path);
    if(result != cgltf_result_success)
        return NULL;

    result = cgltf_validate(data);
    printf("Result: %d\n", result);

    cgltf_mesh* mesh = &data->meshes[0];
    cgltf_primitive* prim = &mesh->primitives[0];

// Extract vertex positions

    cgltf_accessor* pos_accessor = prim->attributes[0].data;
    cgltf_size pos_count = pos_accessor->count;
    float* positions = malloc(pos_count * sizeof(float));
    cgltf_accessor_read_float(pos_accessor, 0, positions, pos_count * 3);

// Extract indices

    cgltf_accessor* index_accessor = prim->indices;
    cgltf_size index_count = index_accessor->count;
    uint32_t* indices = malloc(pos_count * sizeof(uint32_t));
    cgltf_accessor_read_uint(index_accessor, 0, indices, index_count);

// Extract UV coordinates

    cgltf_accessor* uv_accessor = prim->attributes[1].data;
    cgltf_size uv_count = uv_accessor->count;
    float* uvs = malloc(uv_count * sizeof(float));
    cgltf_accessor_read_float(uv_accessor, 0, uvs, uv_count * 2);

    Mesh* ret = loadMeshWithIndices(positions, (long)(pos_count * sizeof(float)), indices, (long)(index_count * sizeof(uint32_t)));

    loadUV(ret, uvs, (long)(uv_count * sizeof(float)));

    cgltf_free(data);
    free(positions);
    free(indices);
    free(uvs);
    return ret;
}

