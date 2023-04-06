#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>

#include "engine/Core/DisplayManager.h"
#include "engine/Core/Shader.h"
#include "engine/Core/ShaderProgram.h"
#include "engine/Core/MeshLoader.h"
#include "engine/Core/Renderer.h"
#include "engine/GameObject/Entity.h"
#include "engine/Utils/Misc.h"
#include "engine/GameObject/Camera.h"
#include "engine/Utils/Primitives.h"
#include "engine/Utils/FileUtils.h"
#include <cglm/cglm.h>

int main() {

    DisplayManager* manager = newDisplayManager(1024, 768);

    BMPImage* text = readBMP("../res/textures/uvtemplate.bmp");

    printf("Image result: width: %d height: %d\n", text->width, text->height);

    vec3 eye = {0, 0, -6};
    vec3 center = {0, 0, 0};

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

    Mesh* mesh = loadMesh(Cube, sizeof(Cube));
    loadUV(mesh, CubeUV, sizeof(CubeUV));
    loadBMPTexture(mesh, text, 1);
    Entity* entity = newEntity(mesh);
    enRotate(entity, 5, (vec3)AXIS_Y);

    Renderer* renderer = newRenderer(45.f, 1024, 768, 0.1f, 100.f);
    Camera* camera = newCamera(eye, center, (vec3) AXIS_Y);

    do {
        prepareRenderer(renderer, program, camera);

        enRotate(entity, (float)(0.5 * manager->deltaTime), (vec3)AXIS_Y);

        useProgram(program);
            renderEntity(renderer, entity, program);
        stopProgram(program);

        update(manager);
    } while(isCloseRequested(manager) == 0);

    dmCleanup(manager);

    // Contains shader cleanup
    shaderProgramCleanup(program);
    free(shaders);

    deleteMesh(mesh);
    entity->mesh = NULL;

    entityCleanup(entity);

    rendererCleanup(renderer);
    cameraCleanup(camera);

    return 0;
}
