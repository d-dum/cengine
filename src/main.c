#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>

#include "engine/Core/DisplayManager.h"
#include "engine/Core/Shader.h"
#include "engine/Core/ShaderProgram.h"

int main() {

    DisplayManager* manager = newDisplayManager(1024, 768);

    static const GLfloat g_vertex_buffer_data[] = {
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

    do {

        update(manager);
    }while(isCloseRequested(manager) == 0);

    dmCleanup(manager);

    // Contains shader cleanup
    shaderProgramCleanup(program);
    free(shaders);

    return 0;
}
