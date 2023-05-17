#version 330 core

in vec2 UV;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

uniform vec3 lightColor;
uniform float shineDamper;
uniform float reflectivity;

void main(){

    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitLightVector = normalize(toLightVector);

    float nDotl = dot(unitNormal, unitLightVector);
    float brightness = max(nDotl, 0.2);
    vec3 diffuse = brightness * lightColor;

    vec3 unitVectorToCamera = normalize(toCameraVector);
    vec3 lightDirection = -unitVectorToCamera;
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

    float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, shineDamper);
    vec3 finalSpecular = dampedFactor * reflectivity * lightColor;

    // Output color = color of the texture at the specified UV
    color = vec4(diffuse, 1.0) * texture( myTextureSampler, UV ).rgba + vec4(finalSpecular, 1.0);
}