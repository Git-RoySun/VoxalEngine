#version 450


layout(location = 0) in vec3 position;
layout(location = 1) in vec3 worldPosition;
layout(location = 2) in float scale;
layout(location = 3) in vec3 rotation;
layout(location = 4) in uint modelId;

layout(location = 0) out vec3 outColor;

layout( push_constant ) uniform constants{
	mat4 view;
} PushConstants;

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
    gl_Position = PushConstants.view * transformMatrix * vec4(position, 1.0);
    outColor = position+vec3(0.5);
}