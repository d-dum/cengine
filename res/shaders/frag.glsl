#version 330 core

in vec2 UV;
in vec3 surfaceNormal;
in vec3 toLightVector;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

uniform vec3 lightColor;

void main(){

    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(toLightVector);

    float nDotl = dot(unitNormal, unitLightVector);
    float brightness = max(nDotl, 0.0);
    vec3 diffuse = brightness * lightColor;

    // Output color = color of the texture at the specified UV
    color = vec4(diffuse, 1.0) * texture( myTextureSampler, UV ).rgba;
}