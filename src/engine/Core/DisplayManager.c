//
// Created by vd on 27.12.22.
//

#include "DisplayManager.h"

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

DisplayManager* newDisplayManager(int width, int height){

    if(!glfwInit()){
        fprintf(stderr, "Failed to initialize GLFW.\n");
        exit(-1);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    DisplayManager* manager = (DisplayManager*) malloc(sizeof(DisplayManager));

    if(manager == NULL){
        fprintf(stderr, "FAILED to allocate memory for DisplayManager\n");
        exit(-1);
    }

    manager->window = glfwCreateWindow(width, height, "Engine test", NULL, NULL);
    if(manager->window == NULL){
        fprintf(stderr, "Failed to open GLFW window.");
        glfwTerminate();
        free(manager);
        exit(-1);
    }
    glfwMakeContextCurrent(manager->window);
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        fprintf(stderr, "Failed to initialize GLEW\n");
        free(manager);
        exit(-1);
    }
    glfwSetInputMode(manager->window, GLFW_STICKY_KEYS, GL_TRUE);
    return manager;
}

int isCloseRequested(DisplayManager* manager){
    return glfwWindowShouldClose(manager->window) == 1;
}

void update(DisplayManager* manager){
    glfwSwapBuffers(manager->window);
    glfwPollEvents();
}

void dmCleanup(DisplayManager* manager){
    glfwDestroyWindow(manager->window);
    glfwTerminate();
    free(manager);
}