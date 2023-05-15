#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normals;

uniform mat4 Model;
uniform mat4 Projection;
uniform mat4 View;

uniform vec3 lightPosition;

out vec2 UV;
out vec3 surfaceNormal;
out vec3 toLightVector;

void main(){
    vec4 worldPosition = Model * vec4(vertexPosition_modelspace, 1);

    gl_Position = Projection * View * worldPosition;
    UV = vertexUV;
    surfaceNormal = (Model * vec4(normals, 0.0)).xyz;
    toLightVector = lightPosition - worldPosition.xyz;
}