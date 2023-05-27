#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define WREN_SCRIPT
#include "engine/engine.h"


#ifndef SCRIPT_TEST
#define SCRIPT_TEST 1
#endif

#if SCRIPT_TEST == 0
int main() {

    srand(time(NULL));

    DisplayManager* manager = newDisplayManager(1024, 768);

    vec3 eye = {0, 0, -20};
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
    loadAnyTexture(mesh, "../res/textures/stallTexture.png");
    Entity* ent = newEntity(mesh);

    enScale(ent, (vec3){0.1, 0.1, 0.1});

    Entity** entities = malloc(sizeof(Entity*) * 10);

    for(int i = 0; i < 10; i++){
        entities[i] = newEntity(mesh);
        //enScale(entities[i], (vec3){0.f, 0.1f, 0.1f});
        float x = (float)rand() / (float)RAND_MAX;
        float y = (float)rand() / (float)RAND_MAX;
        float z = (float)rand() / (float)RAND_MAX;

        enTranslate(entities[i], (vec3){x, y, z});
    }

    MasterRenderer* msr = newMasterRenderer(manager, 45.0f, program);
    msr->entities = entities;
    msr->arrLength = 10;

    printf("Loaded mesh from obj: vao: %d, vbo: %d, ebo: %d, uv: %d, normals: %d\n", mesh->vao, mesh->vbo, mesh->ebo, mesh->uv, mesh->nbo);

    fflush(stdout);
    Renderer* renderer = newRenderer(45.f, 1024, 768, 0.1f, 100.f);
    Camera* camera = newCamera(eye, center, (vec3) AXIS_Y);

    Light* light = newLight((vec3){0.0f, 0.0f, -20.0f}, (vec3){1.f, 1.f, 1.f});

    do {
        prepareRenderer(renderer, program, camera);

        useProgram(program);
            for(int i = 0; i < 10; i++)
                renderEntity(renderer, entities[i], program, light);
        stopProgram(program);
        //renderMaster(msr, light, camera);

        update(manager);
    } while(isCloseRequested(manager) == 0);

    dmCleanup(manager);
    //freeBMP(text);

    // Contains shader cleanup
    shaderProgramCleanup(program);
    free(shaders);

    deleteMesh(mesh);
    ent->mesh = NULL;
    entityCleanup(ent);

    rendererCleanup(renderer);
    cameraCleanup(camera);
    deleteLight(light);
    deleteMasterRenderer(msr, 0, 1);

    return 0;
}
#endif

#if SCRIPT_TEST == 1

int main(void){
    ScriptEngine* engine = newScriptEngine(1);

    executeScript(engine, "../scripts/main.wren", 1, 1);

    destroyScriptEngine(engine);
    return 0;
}

#endif
