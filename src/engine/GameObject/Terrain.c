//
// Created by vd on 15.05.23.
//

#include "Terrain.h"

#include <stdlib.h>

Mesh* generateTerrain(){
    int count = TERRAIN_VERTEX_COUNT * TERRAIN_VERTEX_COUNT;

    float* vertices = calloc(count * 3, sizeof(float));
    float* normals = calloc(count * 3, sizeof(float));
    float* textureCoord = calloc(count * 2, sizeof(float));
    uint* indices = calloc(6 * (TERRAIN_VERTEX_COUNT-1)*(TERRAIN_VERTEX_COUNT-1), sizeof(int));

    int vertexPointer = 0;

    for(int i = 0; i < TERRAIN_VERTEX_COUNT; i++){
        for(int j = 0; j < TERRAIN_VERTEX_COUNT; j++){
            vertices[vertexPointer*3] = (float)j / ((float)TERRAIN_VERTEX_COUNT - 1) * (float) TERRAIN_SIZE;
            vertices[vertexPointer*3+1] = 0;
            vertices[vertexPointer*3+2] = (float)i / ((float)TERRAIN_VERTEX_COUNT - 1) * (float) TERRAIN_SIZE;

            normals[vertexPointer*3] = 0;
            normals[vertexPointer*3+1] = 1;
            normals[vertexPointer*3+2] = 0;

            textureCoord[vertexPointer*2] = (float)j / ((float)TERRAIN_VERTEX_COUNT - 1);
            textureCoord[vertexPointer*2+1] = (float)i / ((float)TERRAIN_VERTEX_COUNT - 1);
            vertexPointer++;
        }
    }

    int pointer = 0;

    for(int gz = 0; gz < TERRAIN_VERTEX_COUNT - 1; gz++){
        for(int gx = 0; gx < TERRAIN_VERTEX_COUNT - 1; gx++){
            int topLeft = (gz * TERRAIN_VERTEX_COUNT) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * TERRAIN_VERTEX_COUNT) + gx;
            int bottomRight = bottomLeft + 1;
            indices[pointer++] = topLeft;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = topRight;
            indices[pointer++] = topRight;
            indices[pointer++] = bottomLeft;
            indices[pointer++] = bottomRight;
        }
    }

    Mesh* ret = loadMeshWithIndices(vertices, count * 3 * sizeof(float), indices, 6 * (TERRAIN_VERTEX_COUNT-1)*(TERRAIN_VERTEX_COUNT-1));
    addNormals(ret, normals, count * 3);
    loadUV(ret, textureCoord, count * 2);

    free(vertices);
    free(normals);
    free(textureCoord);
    free(indices);

    return ret;
}

Terrain* newTerrain(int gridX, int gridZ, char* texturePath){
    Terrain* terrain = malloc(sizeof(Terrain));
    terrain->x = gridX * TERRAIN_SIZE;
    terrain->z = gridZ * TERRAIN_SIZE;
    terrain->mesh = generateTerrain();
    loadAnyTexture(terrain->mesh, texturePath);
}

void deleteTerrain(Terrain* terrain){
    deleteMesh(terrain->mesh);
    terrain->mesh = NULL;
    free(terrain);
}