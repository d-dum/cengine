//
// Created by vd on 28.12.22.
//

#ifndef ENGINE_SHADERPROGRAM_H
#define ENGINE_SHADERPROGRAM_H

#include <GL/glew.h>
#include "../../../lib/cglm/include/cglm/cglm.h"
#include "Shader.h"

typedef struct{
    GLuint programId;
    Shader** shaders;
    long shaderArrayLength;
} ShaderProgram;

ShaderProgram* newShaderProgram(Shader** shaders, long shaderArrayLength);
void shaderProgramCleanup(ShaderProgram* program);
void useProgram(ShaderProgram* program);
void stopProgram(ShaderProgram* program);
int loadMatrix(ShaderProgram* program, vec4* mat, const GLchar* uniformName);
int loadVec3(ShaderProgram* program, float* vec, const GLchar* uniformName);
int loadFloat(ShaderProgram* program, float fl, const GLchar* uniformName);

#endif //ENGINE_SHADERPROGRAM_H
