//
// Created by vd on 17.05.23.
//

#include "MasterRenderer.h"

#include <stdlib.h>

MasterRenderer* newMasterRenderer(DisplayManager* mng, float fov, ShaderProgram* program){
    MasterRenderer* ret = malloc(sizeof(MasterRenderer));
    ret->renderer = newRenderer(fov, (float)mng->height, (float)mng->width, 0.1f, 100.0f);
    ret->shader = program;
}

void renderMaster(MasterRenderer* renderer, Light* light, Camera* camera){
    prepareRenderer(renderer->renderer, renderer->shader, camera);
    useProgram(renderer->shader);
    if(light != NULL){
        loadVec3(renderer->shader, light->position, "lightPosition");
        loadVec3(renderer->shader, light->colour, "lightColor");
    }
    stopProgram(renderer->shader);
}

void deleteMasterRenderer(MasterRenderer* renderer, char delShader, char delEntities){
    rendererCleanup(renderer->renderer);
    if(delShader){
        shaderProgramCleanup(renderer->shader);
    }
    if(delEntities){
        for(size_t i = 0; i < renderer->arrLength; i++){
            entityCleanup(renderer->entities[i]);
        }
        free(renderer->entities);
    }
    renderer->entities = NULL;
    renderer->renderer = NULL;
    renderer->shader = NULL;
    free(renderer);
}