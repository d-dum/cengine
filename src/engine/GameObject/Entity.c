//
// Created by vd on 29.12.22.
//

#include "Entity.h"


#include "../Utils/Mat.h"
#include <stdlib.h>

Entity* newEntity(Mesh* mesh){
    Entity* entity = (Entity*) malloc(sizeof(Entity));
    entity->mesh = mesh;
    entity->mvp = createIdentityMatrix();

    return entity;
}

void enRotate(Entity* entity, float angle, vec3 rotationAxis){
    glm_rotate(entity->mvp, angle, rotationAxis);
}

void enTranslate(Entity* entity, vec3 transform){
    glm_translate(entity->mvp, transform);
}

void enScale(Entity* entity, vec3 scale){
    glm_scale(entity->mvp, scale);
}

void enIncreasePosition(float d, vec3 axis){

}

void enIncreaseRotation(float d, vec3 axis){

}

void entityCleanup(Entity* entity){
    freeMatrixVector(entity->mvp);
    free(entity);
}