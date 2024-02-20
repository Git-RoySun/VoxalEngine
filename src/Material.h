#pragma once
#include <glm/vec3.hpp>
typedef uint16_t matId;

struct Material {
  alignas(4) glm::u8vec3 colour;

  //todo add textures

  //todo implement PBR
  //float roughtness;
  //float metallic;
  //float specular;
  //float transparency;
};
