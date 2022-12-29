//
// Created by vd on 29.12.22.
//

#include "Entity.h"


#include "../Utils/Mat.h"
#include <stdlib.h>

#include <cglm/cglm.h>

Entity* newEntity(Mesh* mesh){
    Entity* entity = (Entity*) malloc(sizeof(Entity));
    entity->mesh = mesh;
    entity->mvp = createIdentityMatrix();

    return entity;
}

void enRotate(Entity* entity, float angle, vec3 rotation){

}

void enTranslate(Entity* entity, vec3 transform){

}

void entityCleanup(Entity* entity){
    freeMatrixVector(entity->mvp);
    free(entity);
}