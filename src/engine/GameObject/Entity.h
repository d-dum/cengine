//
// Created by vd on 29.12.22.
//

#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include "../Core/MeshLoader.h"

#include <cglm/cglm.h>

typedef struct {
    Mesh* mesh;
    vec4* mvp;
} Entity;

Entity* newEntity(Mesh* mesh);

void entityCleanup(Entity* entity);

#endif //ENGINE_ENTITY_H
