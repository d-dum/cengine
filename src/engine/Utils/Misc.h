//
// Created by vd on 29.12.22.
//

#ifndef ENGINE_MISC_H
#define ENGINE_MISC_H

#include <GL/glew.h>

#ifndef MISC_AXIS
#define MISC_AXIS

#define AXIS_X {1.f, 0.f, 0.f}
#define AXIS_Y {0.f, 1.f, 0.f}
#define AXIS_Z {0.f, 0.f, 1.f}

#endif

#ifndef MISC_SHADER
#define MISC_SHADER

#define VERTEX_SHADER GL_VERTEX_SHADER
#define FRAGMENT_SHADER GL_FRAGMENT_SHADER

#endif

#endif //ENGINE_MISC_H
