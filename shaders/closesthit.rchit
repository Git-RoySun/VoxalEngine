#version 460

#extension GL_EXT_ray_tracing : require
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_EXT_buffer_reference2 : require
#extension GL_EXT_scalar_block_layout : require
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require

struct Material {
    vec3 colour;
    float albedo;
    float roughness;
    float alpha;
};

struct Instance {
    vec3 position;
    vec3 scale;
    vec3 rotation;
    uint matId;
};

layout(location = 0) rayPayloadInEXT vec3 hitValue;
layout(binding = 0, set = 0) uniform accelerationStructureEXT topLevelAS;
layout(set = 0, binding = 2) uniform CameraProperties 
{
	mat4 view;
	mat4 proj;

	vec4  clearColor;
	vec3  lightPosition;
	float lightIntensity;
} cam;
layout(set = 0, binding = 3) buffer InstanceBuffer { Instance instances[]; };
layout(set = 0, binding = 4) buffer MaterialBuffer { Material materials[]; };

void main(){
  const vec3 worldPos = gl_WorldRayOriginEXT + gl_WorldRayDirectionEXT * gl_HitTEXT;
  const vec3 cr = instances[gl_InstanceID].rotation;
  const mat3 rotationMatrix = mat3(
      cos(cr.y) * cos(cr.z), -sin(cr.y) * cos(cr.x) + cos(cr.y) * sin(cr.z) * sin(cr.x), sin(cr.y) * sin(cr.x) + cos(cr.y) * sin(cr.z) * cos(cr.x),
      sin(cr.y) * cos(cr.z), cos(cr.y) * cos(cr.x) + sin(cr.y) * sin(cr.z) * sin(cr.x), -cos(cr.y) * sin(cr.x) + sin(cr.y) * sin(cr.z) * cos(cr.x),
      -sin(cr.z), cos(cr.z) * sin(cr.x), cos(cr.z) * cos(cr.x)
  );

  // Calculate the point in the cube's local space
  const vec3 localPos = (worldPos - instances[gl_InstanceID].position) * inverse(rotationMatrix);
  const vec3 absPoint = abs(localPos); 
  const vec3 signs = sign(localPos);
  const vec3 normal = (absPoint.x > absPoint.y && absPoint.x > absPoint.z) ? rotationMatrix[0] * signs.x : (absPoint.y > absPoint.z) ? rotationMatrix[1] * signs.y : rotationMatrix[2] * signs.z;

  // Vector toward the light
  vec3 L;
  float lightIntensity = cam.lightIntensity;
  float lightDistance  = 100000.0;
  L = normalize(cam.lightPosition);

  // Material of the object
  const Material mat = materials[instances[gl_InstanceID].matId];

  // Diffuse
  float diffuse     = (dot(normal, L)+1)/2;
  float attenuation = 0.3;

  // Tracing shadow ray only if the light is visible from the surface
  if(dot(normal, L) > 0){
    float tMin   = 0.001;
    float tMax   = lightDistance;
    vec3  rayDir = L;
    uint  flags  = gl_RayFlagsTerminateOnFirstHitEXT | gl_RayFlagsOpaqueEXT | gl_RayFlagsSkipClosestHitShaderEXT;
    traceRayEXT(topLevelAS,  // acceleration structure
                flags,       // rayFlags
                0xFF,        // cullMask
                0,           // sbtRecordOffset
                0,           // sbtRecordStride
                1,           // missIndex
                worldPos,    // ray origin
                tMin,        // ray min range
                rayDir,      // ray direction
                tMax,        // ray max range
                0            // payload (location = 1)
    );
  }
  vec3 colour = mat.colour;

  hitValue = vec3(lightIntensity * attenuation * diffuse * colour);
}
