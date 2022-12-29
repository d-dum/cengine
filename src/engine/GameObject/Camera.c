//
// Created by vd on 29.12.22.
//

#include "Camera.h"

#include "../Utils/Mat.h"

#include <stdlib.h>

Camera* newCamera(vec3 position, vec3 lookAt, vec3 up){
    Camera* camera = (Camera*) malloc(sizeof(Camera));
    camera->lookAt = lookAt;
    camera->position = position;
    camera->up = up;
    camera->view = createCameraMatrix(position, lookAt, up);
    return camera;
}


void updateView(Camera* camera){
    camera->view = createCameraMatrix(camera->position, camera->lookAt, camera->up);
}


void cameraCleanup(Camera* camera){
    freeMatrixVector(camera->view);
    free(camera);
}
