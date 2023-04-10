#include <stdlib.h>
#include <stdio.h>

#include "engine/engine.h"

int main() {

    DisplayManager* manager = newDisplayManager(1024, 768);

    BMPImage* text = readBMP("../res/textures/stallTexture.bmp");
//    PNGImage* texture = readPNGNew("../res/textures/stallTexture.png");

    printf("Image result: width: %d height: %d\n", text->width, text->height);

    vec3 eye = {0, 0, -6};
    vec3 center = {0, 0, 0};

    Shader* vertex = newShader("../res/shaders/vert.glsl", VERTEX_SHADER);
    Shader* fragment = newShader("../res/shaders/frag.glsl", FRAGMENT_SHADER);

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

    Mesh* mesh = loadFromOBJ("../res/models/stall.obj", 1);
    //loadBMPTexture(mesh, text, 0);
//    loadPNGTexture(mesh, texture, 1);
    loadBMPTexture(mesh, text, 0);
    Entity* ent = newEntity(mesh);

    enScale(ent, (vec3){0.1, 0.1, 0.1});

    printf("Loaded mesh from obj: vao: %d, vbo: %d, ebo: %d, uv: %d", mesh->vao, mesh->vbo, mesh->ebo, mesh->uv);

    fflush(stdout);
    Renderer* renderer = newRenderer(45.f, 1024, 768, 0.1f, 100.f);
    Camera* camera = newCamera(eye, center, (vec3) AXIS_Y);

    do {
        prepareRenderer(renderer, program, camera);

        enRotate(ent, (float)(0.5 * manager->deltaTime), (vec3)AXIS_Y);

        useProgram(program);
            renderEntity(renderer, ent, program);
        stopProgram(program);

        update(manager);
    } while(isCloseRequested(manager) == 0);

    dmCleanup(manager);
    freeBMP(text);

    // Contains shader cleanup
    shaderProgramCleanup(program);
    free(shaders);

    deleteMesh(mesh);
    ent->mesh = NULL;
    entityCleanup(ent);

    rendererCleanup(renderer);
    cameraCleanup(camera);

    return 0;
}
