#version 450
struct Material {
    vec3 colour;
    float albedo;
    float alpha;
};


layout(location = 0) in vec3 position;
layout(location = 1) in vec3 worldPosition;
layout(location = 2) in vec3 scale;
layout(location = 3) in vec3 rotation;
layout(location = 4) in uint modelId;

layout(location = 0) out vec3 outColor;


layout(set = 0, binding =0) uniform Ubo{
    mat4 view;
    vec3 lightDirection;
} ubo;

layout(set = 0, binding = 1) buffer MaterialBuffer {
    Material materials[];
};

float c3 = cos(rotation.z);
float s3 = sin(rotation.z);
float c2 = cos(rotation.x);
float s2 = sin(rotation.x);
float c1 = cos(rotation.y);
float s1 = sin(rotation.y);

mat4 transformMatrix = mat4(
    vec4(
        scale.x* (c1* c3 + s1 * s2 * s3),
        scale.x* (c2* s3),
        scale.x* (c1* s2* s3 - c3 * s1),
        0.0f
    ),
    vec4(
        scale.y * (c3 * s1 * s2 - c1 * s3),
        scale.y * (c2 * c3),
        scale.y * (c1 * c3 * s2 + s1 * s3),
        0.0f
    ), 
    vec4(
        scale.z * (c2 * s1),
        scale.z * (-s2),
        scale.z * (c1 * c2),
        0.0f
    ),
    vec4( worldPosition.x, worldPosition.y, worldPosition.z, 1.0f)
);


void main() {
    gl_Position = ubo.view * transformMatrix * vec4(position, 1.0);
    outColor = vec3(position+vec3(1.0))/2;//materials[modelId].colour
}