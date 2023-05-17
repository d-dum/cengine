//
// Created by vd on 17.05.23.
//

#ifndef ENGINE_MASTERRENDERER_H
#define ENGINE_MASTERRENDERER_H

#include "ShaderProgram.h"
#include "Renderer.h"
#include "../GameObject/Entity.h"
#include "../GameObject/Camera.h"
#include "../GameObject/Light.h"
#include "DisplayManager.h"

/// entities must have same mesh (including texture)
typedef struct MasterRenderer {
    ShaderProgram* shader;
    Renderer* renderer;
    Entity** entities; // Array of pointers to entity struct
    size_t arrLength;
} MasterRenderer;

MasterRenderer* newMasterRenderer(DisplayManager* mng, float fov, ShaderProgram* program);
void renderMaster(MasterRenderer* renderer, Light* light, Camera* camera);

/// Assumes entities are allocated dynamically
void deleteMasterRenderer(MasterRenderer* renderer, char delShader, char delEntities);

#endif //ENGINE_MASTERRENDERER_H
