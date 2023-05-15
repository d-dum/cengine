//
// Created by vd on 15.05.23.
//

#include "Light.h"


#include <stdlib.h>


Light* newLight(vec3 position, vec3 color){
    Light* ret = malloc(sizeof(Light));
    ret->colour = color;
    ret->position = position;

    return ret;
}

void loadLight(Light* light, GLuint colorLocation, GLuint posLocation){
    glUniform3fv(colorLocation, 1, light->colour);
    glUniform3fv(posLocation, 1, light->position);
}

void deleteLight(Light* light){
    free(light);
}