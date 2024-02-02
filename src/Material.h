#pragma once
#include <glm/vec3.hpp>
typedef uint32_t matId;

struct Material {
  alignas(16) glm::vec3 colour;

  //todo add textures

  //todo implement PBR
  //float roughtness;
  //float metallic;
  //float specular;
  //float transparency;
};
