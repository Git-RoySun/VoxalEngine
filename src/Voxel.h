#pragma once
#include "Material.h"
#include "Object.h"

namespace obj {
  class Voxel: public Base {
  public:
    struct Instance {
      glm::mat4 transform{};
      matId     materialID = 0;
    };

  private:
    uint32_t materialID;

  public:
    Voxel(Transform transform = {}, uint32_t matId = 0): Base{transform}, materialID{matId} {};

    Instance toInstance() const {
      return Instance{transform.mat4(), materialID};
    }
  };
}
