//
// Created by vd on 27.12.22.
//

#ifndef ENGINE_SHADER_H
#define ENGINE_SHADER_H

#include <GL/glew.h>

typedef struct Shader {
    GLuint shaderId;
    GLenum type;
} Shader;

Shader* newShader(char* shaderFilePath, GLenum type);
void shaderCleanup(Shader* shader);

#endif //ENGINE_SHADER_H
