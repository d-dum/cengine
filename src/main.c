#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>

#include "engine/Core/DisplayManager.h"
#include "engine/Core/Shader.h"
#include "engine/Core/ShaderProgram.h"
#include "engine/Core/MeshLoader.h"
#include "engine/Core/Renderer.h"
#include "engine/Utils/Mat.h"
#include "engine/GameObject/Entity.h"
#include <cglm/cglm.h>

int main() {

    DisplayManager* manager = newDisplayManager(1024, 768);



    vec3 eye = {0, 0, -6};
    vec3 center = {0, 0, 0};
    vec3 up = {0, 1, 0};

    vec4* cam = createCameraMatrix(eye, center, up);



    static GLfloat g_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
    };

    Shader* vertex = newShader("../res/shaders/vert.glsl", GL_VERTEX_SHADER);
    Shader* fragment = newShader("../res/shaders/frag.glsl", GL_FRAGMENT_SHADER);

    if(vertex == NULL || fragment == NULL){
        dmCleanup(manager);
        if(vertex != NULL) shaderCleanup(vertex);
        if(fragment != NULL) shaderCleanup(fragment);
        return -1;
    }

    Shader** shaders = malloc(sizeof(Shader*)*2);
    shaders[0] = vertex;
    shaders[1] = fragment;

    ShaderProgram* program = newShaderProgram(shaders, 2);

    if(program == NULL){
        shaderCleanup(shaders[0]);
        shaderCleanup(shaders[1]);
        dmCleanup(manager);
        free(shaders);
        return -1;
    }

    fflush(stdout);

    Mesh* mesh = loadMesh(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
    Entity* entity = newEntity(mesh);

    Renderer* renderer = newRenderer(45.f, 1024, 768, 0.1f, 100.f);

    do {
        prepareRenderer(renderer, program);

        useProgram(program);
            loadMatrix(program, cam, "View");
            loadMatrix(program, entity->mvp, "Model");

            renderMesh(mesh);
        stopProgram(program);

        update(manager);
    } while(isCloseRequested(manager) == 0);

    dmCleanup(manager);

    // Contains shader cleanup
    shaderProgramCleanup(program);
    free(shaders);

    freeMatrixVector(cam);

    deleteMesh(mesh);
    entity->mesh = NULL;

    entityCleanup(entity);

    rendererCleanup(renderer);

    return 0;
}
