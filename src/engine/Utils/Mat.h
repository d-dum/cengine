//
// Created by vd on 28.12.22.
//

#ifndef ENGINE_MAT_H
#define ENGINE_MAT_H

#include <cglm/cglm.h>

vec4* createMVPMatrix();
void printMatrix(vec4* mat);
void freeMatrixVector(void* mat);
vec4* createPerspectiveMatrix(float fov, float width, float height, float near, float far);
vec4* createCameraMatrix(vec3 eye, vec3 center, vec3 up);
vec4* createModelMatrix(vec4* projection, vec4* view);

#endif //ENGINE_MAT_H
