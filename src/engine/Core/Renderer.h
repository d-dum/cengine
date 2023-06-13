//
// Created by vd on 28.12.22.
//

#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include "MeshLoader.h"
#include "ShaderProgram.h"
#include "../GameObject/Camera.h"
#include "../GameObject/Entity.h"
#include "../GameObject/Light.h"
#include "../GameObject/Terrain.h"

#include "../../../lib/cglm/include/cglm/cglm.h"

typedef struct Renderer {
    vec4* ortho;
    vec4* projection;
    float fov;
    float width;
    float height;
    float near;
    float far;
} Renderer;

Renderer* newRenderer(float fov, float height, float width, float near, float far);

void updateProjection(Renderer* renderer);

void rendererCleanup(Renderer* renderer);

void prepareRenderer(Renderer* renderer, ShaderProgram* program, Camera* camera);

void renderMesh(Renderer* renderer, Mesh* mesh, ShaderProgram* shaderProgram);

void renderEntity(Renderer* renderer, Entity* entity, ShaderProgram* program, Light* light);

void renderEntities(Renderer* renderer, Entity** entities, ShaderProgram* program, size_t count);

#endif //ENGINE_RENDERER_H
