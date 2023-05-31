//
// Created by vd on 15.05.23.
//

#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H
#include <GL/glew.h>
#include "../../../lib/cglm/include/cglm/cglm.h"

typedef struct Light {
    float* position;
    float* colour;
} Light;

Light* newLight(vec3 position, vec3 color);
void loadLight(Light* light, GLuint colorLocation, GLuint posLocation);
void deleteLight(Light* light);

#endif //ENGINE_LIGHT_H
