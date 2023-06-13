//
// Created by vd on 28.12.22.
//

#include "Mat.h"

#include "../../../lib/cglm/include/cglm/cglm.h"
#include <GL/glew.h>

#include "Mem.h"



void freeMatrixVector(void* mat){
    aligned_free(mat);
}

void printMatrix(vec4* mat){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%f ", mat[i][j]);
        }
        printf("\n");
    }
}

vec4* createPerspectiveMatrix(float fov, float width, float height, float near, float far){
    vec4* mat = MAT4ALLOC;
    glm_perspective(glm_rad(fov), width / height, near, far, mat);
    return mat;
}

vec4* createOrthoMatrix(float left, float right, float bottom, float top, float near, float far){
    vec4* mat = MAT4ALLOC;
    glm_ortho(left, right, bottom, top, near, far, mat);
    return mat;
}

vec4* createCameraMatrix(vec3 eye, vec3 center, vec3 up){
    vec4* mat = MAT4ALLOC;
    glm_lookat(eye, center, up, mat);
    return mat;
}

vec4* createModelMatrix(vec4* projection, vec4* view){
    vec4* model = MAT4ALLOC;
    glm_mat4_identity(model);
    vec4* mvp = MAT4ALLOC;
    glm_mat4_mul(projection, view, mvp);
    glm_mat4_mul(mvp, model, mvp);
    freeMatrixVector(model);
    return mvp;
}

vec4* createIdentityMatrix(){
    vec4* mat = MAT4ALLOC;
    glm_mat4_identity(mat);
    return mat;
}

