//
// Created by vd on 28.12.22.
//

#include "Renderer.h"

#include <GL/glew.h>
#include <stdlib.h>

#include "../Utils/Mat.h"
#include "ShaderProgram.h"

void prepareRenderer(Renderer* renderer, ShaderProgram* program){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 1, 1);
    useProgram(program);
    loadMatrix(program, renderer->projection, "Projection");
    stopProgram(program);
}

void renderMesh(Mesh* mesh){
    glBindVertexArray(mesh->vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*) 0
            );
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Renderer* newRenderer(float fov, float height, float width, float near, float far){
    Renderer* renderer = (Renderer*) malloc(sizeof(Renderer));
    renderer->projection = createPerspectiveMatrix(fov, width, height, near, far);
    renderer->near = near;
    renderer->height = height;
    renderer->width = width;
    renderer->fov = fov;
    renderer->far = far;

    return renderer;
}

void updateProjection(Renderer* renderer){
    renderer->projection = createPerspectiveMatrix(
            renderer->fov,
            renderer->width,
            renderer->height,
            renderer->near,
            renderer->far
            );
}

void rendererCleanup(Renderer* renderer){
    freeMatrixVector(renderer->projection);
    free(renderer);
}