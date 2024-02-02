#version 450
precision mediump float;       // Set the default precision to medium. We don't need as high of a
                               // precision in the fragment shader.

layout(location = 0) in vec3 color;
layout(location = 1) in vec3 position;
layout(location = 2) flat in vec3 normal;

layout(location = 0) out vec4 outColor;

const vec3 lightDir = vec3(16.0,-5.0,1.0);
void main() {
   float distance = LENGTH(lightDir-position);

    // Get a lighting direction vector from the light to the vertex.
    vec3 lightVector = normalize(lightDir - position);
 
    // Calculate the dot product of the light vector and vertex normal. If the normal and light vector are
    // pointing in the same direction then it will get max illumination.
    float diffuse = max(dot(normal, lightVector), 0.1);
 
    // Add attenuation.
    diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));
 
    outColor = vec4(color * diffuse, 0);
}