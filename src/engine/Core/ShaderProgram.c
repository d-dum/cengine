//
// Created by vd on 28.12.22.
//

#include "ShaderProgram.h"

#include "Shader.h"

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

ShaderProgram* newShaderProgram(Shader** shaders, long shaderArrayLength){
    GLuint programID = glCreateProgram();
    printf("Linking program: %d\n", programID);
    for(int i = 0; i < shaderArrayLength; i++){
        printf("Attaching shader %d to program %d\n", shaders[i]->shaderId, programID);
        glAttachShader(programID, shaders[i]->shaderId);
    }
    glLinkProgram(programID);
    GLint result = GL_FALSE;
    int infoLogLength;
    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0){
        char* log = (char*)malloc(sizeof(char)*(infoLogLength+1));
        glGetProgramInfoLog(programID, infoLogLength, NULL, &log[0]);
        fprintf(stderr, "FAILED TO LINK PROGRAM %d ERROR MESSAGE: %s\n", programID, log);
        free(log);
        return NULL;
    }
    ShaderProgram* program = (ShaderProgram*) malloc(sizeof(ShaderProgram));
    program->programId = programID;
    program->shaders = shaders;
    program->shaderArrayLength = shaderArrayLength;
    return program;
}

void useProgram(ShaderProgram* program){
    glUseProgram(program->programId);
}

void stopProgram(ShaderProgram* program){
    glUseProgram(0);
}

void shaderProgramCleanup(ShaderProgram* program){
    for(int i = 0; i < program->shaderArrayLength; i++){
        glDetachShader(program->programId, program->shaders[i]->shaderId);
        shaderCleanup(program->shaders[i]);
    }
    glDeleteProgram(program->programId);
    free(program);
}

void shaderProgramCleanupSimple(ShaderProgram* program){
    for(int i = 0; i < program->shaderArrayLength; i++){
        glDetachShader(program->programId, program->shaders[i]->shaderId);
    }
    glDeleteProgram(program->programId);
    free(program);
}

GLint getUniformLocation(ShaderProgram* program, const GLchar* uniformName){
    GLint location = glGetUniformLocation(program->programId, uniformName);
    if(location == -1){
        fprintf(stderr, "Location of uniform %s not found\n", uniformName);
        return -1;
    }
    return location;
}

int loadMatrix(ShaderProgram* program, vec4* mat, const GLchar* uniformName){
    GLint location = glGetUniformLocation(program->programId, uniformName);
    if(location == -1){
        fprintf(stderr, "Location of uniform %s not found\n", uniformName);
        return 0;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, mat[0]);
    return 1;
}

int loadVec3(ShaderProgram* program, float* vec, const GLchar* uniformName){
    GLint location = glGetUniformLocation(program->programId, uniformName);
    if(location == -1){
        fprintf(stderr, "Location of uniform %s not found\n", uniformName);
        return 0;
    }
    glUniform3fv(location, 1, vec);
    return 1;
}

int loadFloat(ShaderProgram* program, float fl, const GLchar* uniformName){
    GLint loc = getUniformLocation(program, uniformName);
    if(loc == -1)
        return -1;
    glUniform1f(loc, fl);
}