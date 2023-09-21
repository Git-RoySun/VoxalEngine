#pragma once
#include "glm/gtc/matrix_transform.hpp"

using id_t = unsigned int;

namespace obj {
  struct Transform {
    glm::vec3 position{};
    glm::vec3 scale{ 1.f };
    glm::vec3 rotation{};
  };

  class Base {
  protected:
    static id_t nextId;
    id_t instanceId;
    Transform transform{};
  public:
    Base(const Base&) = delete;
    Base& operator=(const Base&) = delete;
    Base(Base&&) = default;
    Base& operator=(Base&&) = default;

    Base(Transform transform = Transform{});
    id_t Id() const { return instanceId; };
  };
}

