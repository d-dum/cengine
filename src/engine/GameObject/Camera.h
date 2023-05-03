//
// Created by vd on 29.12.22.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "../../../lib/cglm/include/cglm/cglm.h"

typedef struct {
    vec4* view;
    float* position;
    float* lookAt;
    float* up;
} Camera;

Camera* newCamera(vec3 position, vec3 lookAt, vec3 up);
void updateView(Camera* camera);
void cameraCleanup(Camera* camera);

#endif //ENGINE_CAMERA_H
