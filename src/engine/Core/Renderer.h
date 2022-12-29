//
// Created by vd on 28.12.22.
//

#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include "MeshLoader.h"
#include "ShaderProgram.h"
#include "../GameObject/Entity.h"

#include <cglm/cglm.h>

typedef struct {
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

void prepareRenderer(Renderer* renderer, ShaderProgram* program);

void renderMesh(Renderer* renderer, Mesh* mesh);

void renderEntity(Renderer* renderer, Entity* entity, ShaderProgram* program);

#endif //ENGINE_RENDERER_H
