//
// Created by vd on 27.12.22.
//

#ifndef ENGINE_DISPLAYMANAGER_H
#define ENGINE_DISPLAYMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct {
    int width;
    int height;
    GLFWwindow* window;
    double lastTime;
    double deltaTime;
} DisplayManager;

DisplayManager* newDisplayManager(int width, int height);
void dmCleanup(DisplayManager* manager);
int isCloseRequested(DisplayManager* manager);
void update(DisplayManager* manager);

#endif //ENGINE_DISPLAYMANAGER_H
