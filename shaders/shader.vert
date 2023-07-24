#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 outColor;

layout(set = 0, binding =0) uniform Ubo{
    mat4 view;
    vec3 lightDirection;
} ubo;

layout(push_constant) uniform Push {
    mat4 transform;
} push;

void main() {
    gl_Position = ubo.view * push.transform * vec4(position,1.0);
    outColor = color;
}