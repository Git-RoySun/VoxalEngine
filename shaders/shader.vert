#version 450
struct Material {
    vec3 colour;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 worldPosition;
layout(location = 3) in float scale;
layout(location = 4) in vec3 rotation;
layout(location = 5) in uint matId;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outPosition;
layout(location = 2) flat out vec3 outNormal;

layout( push_constant ) uniform constants{
	mat4 projection;
    mat4 view;
    uint time;
} PushConstants;

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
        scale* (c1* c3 + s1 * s2 * s3),
        scale* (c2* s3),
        scale* (c1* s2* s3 - c3 * s1),
        0.0f
    ),
    vec4(
        scale * (c3 * s1 * s2 - c1 * s3),
        scale * (c2 * c3),
        scale * (c1 * c3 * s2 + s1 * s3),
        0.0f
    ), 
    vec4(
        scale * (c2 * s1),
        scale * (-s2),
        scale * (c1 * c2),
        0.0f
    ),
    vec4( worldPosition.x, worldPosition.y, worldPosition.z, 1.0f)
);

void main() {
    gl_Position = PushConstants.projection * PushConstants.view * transformMatrix * vec4(position, 1.0);

    outNormal = vec3(PushConstants.projection * transformMatrix * vec4(normal, 0.0));
    outPosition = vec3(PushConstants.projection * transformMatrix * vec4(position, 1.0));
    //using PushConstants.view instead of view gives an iridescent effect
    outColor = materials[matId].colour;
}