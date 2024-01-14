#pragma once

namespace obj {
  class Rotary {
  public:
    virtual glm::vec3& getRotation() = 0;

    virtual void rotate(glm::vec3 move) = 0;
    virtual void setRotation(glm::vec3 pos) = 0;
  };
}
