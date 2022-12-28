//
// Created by vd on 27.12.22.
//

#include "Shader.h"

#include "../Utils/FileUtils.h"

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

Shader* newShader(char* shaderFilePath, GLenum type){
    char* source = readFile(shaderFilePath);
    if(source == NULL){
        return NULL;
    }


    GLuint shaderId = glCreateShader(type);

    GLint result = GL_FALSE;
    int infoLogLength;
    printf("Compiling shader: %s\n", shaderFilePath);
    glShaderSource(shaderId, 1, (const GLchar *const *) &source, NULL);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if(infoLogLength > 0){
        char* log = (char*)malloc(sizeof(char) * (infoLogLength+1));
        glGetShaderInfoLog(shaderId, infoLogLength, NULL, &log[0]);
        fprintf(stderr, "ERROR COMPILING SHADER: %s, ERROR MESSAGE: %s", shaderFilePath, log);
        free(log);
        free(source);
        return NULL;
    }

    Shader* shader = (Shader*) malloc(sizeof(Shader));
    shader->shaderId = shaderId;
    shader->type = type;

    free(source);

    return shader;
}


void shaderCleanup(Shader* shader){
    glDeleteShader(shader->shaderId);
    free(shader);
}