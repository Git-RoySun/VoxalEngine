#pragma once

#include <memory>
#include "Model.h"
#include "glm/gtc/matrix_transform.hpp"
namespace vc { class RenderSystem; };
using id_t = unsigned int;
struct Transform {
	glm::vec3 position {};
	glm::vec3 scale {1.f};
	glm::vec3 rotation {};

  glm::mat4 mat4() const {
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    return glm::mat4{
      {
        scale.x* (c1* c3 + s1 * s2 * s3),
          scale.x* (c2* s3),
          scale.x* (c1* s2* s3 - c3 * s1),
          0.0f,
      },
      {
          scale.y * (c3 * s1 * s2 - c1 * s3),
          scale.y * (c2 * c3),
          scale.y * (c1 * c3 * s2 + s1 * s3),
          0.0f,
      },
      {
          scale.z * (c2 * s1),
          scale.z * (-s2),
          scale.z * (c1 * c2),
          0.0f,
      },
      { position.x, position.y, position.z, 1.0f }};
  }
};

class Object{
  friend class vc::RenderSystem;
	static id_t nextId;
	id_t instanceId;
protected:
  Transform transform{};
  std::unique_ptr<vc::Model> model = nullptr;
public:
  Object(const Object&) = delete;
  Object& operator=(const Object&) = delete;
  Object(Object&&) = default;
  Object& operator=(Object&&) = default;

  Object();
	Object(std::unique_ptr<vc::Model> model, Transform transform = Transform{});
	id_t Id() const { return instanceId; };
  glm::mat4 getTransform() const { return transform.mat4(); };
};


