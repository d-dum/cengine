//
// Created by vd on 28.12.22.
//

#ifndef ENGINE_SHADERPROGRAM_H
#define ENGINE_SHADERPROGRAM_H

#include <GL/glew.h>

#include "Shader.h"

typedef struct{
    GLuint programId;
    Shader** shaders;
    long shaderArrayLength;
} ShaderProgram;

ShaderProgram* newShaderProgram(Shader** shaders, long shaderArrayLength);
void shaderProgramCleanup(ShaderProgram* program);

#endif //ENGINE_SHADERPROGRAM_H
