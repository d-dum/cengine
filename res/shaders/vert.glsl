#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

out vec3 fragmentColor;

void main(){
    gl_Position = Projection * View * Model * vec4(vertexPosition_modelspace, 1);
    fragmentColor = vertexColor;
}