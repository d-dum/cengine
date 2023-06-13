//
// Created by vd on 28.12.22.
//

#ifndef ENGINE_MAT_H
#define ENGINE_MAT_H

#include "../../../lib/cglm/include/cglm/cglm.h"

#ifndef MAT4ALLOC

#define MAT4ALLOC (vec4*)aligned_alloc_custom(16, sizeof(mat4), 0)

#endif

#ifdef VEC3ALLOC

#define VEC3ALLOC (float*)aligned_alloc_custom(16, sizeof(vec3), 0)

#endif

vec4* createMVPMatrix();
void printMatrix(vec4* mat);
void freeMatrixVector(void* mat);
vec4* createPerspectiveMatrix(float fov, float width, float height, float near, float far);
vec4* createCameraMatrix(vec3 eye, vec3 center, vec3 up);
vec4* createModelMatrix(vec4* projection, vec4* view);
vec4* createIdentityMatrix();
vec4* createOrthoMatrix(float left, float right, float bottom, float top, float near, float far);
#endif //ENGINE_MAT_H
