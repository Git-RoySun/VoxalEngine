#version 460
#extension GL_EXT_ray_tracing : enable

layout(location = 0) rayPayloadInEXT vec3 hitValue;
layout(binding = 2, set = 0) uniform CameraProperties 
{
	mat4 view;
	mat4 proj;

	vec4  clearColor;
	vec3  lightPosition;
	float lightIntensity;
} cam;

void main(){
    hitValue = vec3(cam.clearColor.x,cam.clearColor.y,cam.clearColor.z);
}