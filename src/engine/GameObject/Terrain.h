//
// Created by vd on 15.05.23.
//

#ifndef ENGINE_TERRAIN_H
#define ENGINE_TERRAIN_H

#include "../Core/MeshLoader.h"

#ifndef TERRAIN_SIZE
#define TERRAIN_SIZE 800
#endif

#ifndef TERRAIN_VERTEX_COUNT
#define TERRAIN_VERTEX_COUNT 128
#endif

typedef struct{
    int x;
    int z;
    Mesh* mesh;
} Terrain;

Terrain* newTerrain(int gridX, int gridZ, char* texturePath);
void deleteTerrain(Terrain* terrain);

#endif //ENGINE_TERRAIN_H
