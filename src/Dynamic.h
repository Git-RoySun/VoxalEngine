#pragma once
#include <glm/vec3.hpp>

namespace obj {
  class Dynamic {
  public:
    virtual glm::vec3& getPosition() = 0;

    virtual void move(glm::vec3 move) = 0;
    virtual void setPosition(glm::vec3 pos) = 0;
  };
}
