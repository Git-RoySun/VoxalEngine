#pragma once
#include "glm/glm.hpp"
#include <cstdint>

namespace obj{
  struct Transform {
    glm::vec3 position{0.f};
    glm::vec3 scale{ 1.f };
    glm::vec3 rotation{0.f};
  	glm::mat4 mat4() const {
      const float c3 = glm::cos(rotation.z);
      const float s3 = glm::sin(rotation.z);
      const float c2 = glm::cos(rotation.x);
      const float s2 = glm::sin(rotation.x);
      const float c1 = glm::cos(rotation.y);
      const float s1 = glm::sin(rotation.y);
      return glm::mat4{
        {
          scale.x * (c1 * c3 + s1 * s2 * s3),
            scale.x * (c2 * s3),
            scale.x * (c1 * s2 * s3 - c3 * s1),
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
        { position.x, position.y, position.z, 1.0f } };
    }
  };

  class Base {
  protected:
    static uint32_t nextId;
    uint32_t instanceId;
    Transform transform{};
  public:
    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;
    Base(Base&&) = default;
    Base& operator=(Base&&) = default;

    Base(Transform transform = {});
    uint32_t Id() const { return instanceId; };
  };
}

