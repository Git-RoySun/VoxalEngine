#version 450
struct Material {
    vec3 colour;
};

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in mat4 transformMatrix;
layout(location = 6) in float random;
layout(location = 7) in uint matId;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outPosition;
layout(location = 2) flat out vec3 outNormal;
layout(location = 3) flat out vec3 outLight;

layout(set = 0, binding = 0) uniform Ubo{
    mat4 projection;
    mat4 view;
    vec3 lightDirection;
} ubo;

layout(set = 0, binding = 1) buffer MaterialBuffer {
    Material materials[];
};

void main() {
    gl_Position = ubo.projection * ubo.view * transformMatrix * vec4(position, 1.0);

    outNormal = normalize(vec3(ubo.projection * transformMatrix * vec4(normal, 0.0)));
    outPosition = vec3(ubo.projection *  transformMatrix * vec4(position, 1.0));
    //using PushConstants.view instead of view gives an iridescent effect
    outLight = vec3(ubo.projection *  transformMatrix * vec4(ubo.lightDirection, 1.0));
    outColor = materials[matId].colour * (random*0.75+0.25);
}