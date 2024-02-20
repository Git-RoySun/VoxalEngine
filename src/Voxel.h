#pragma once
#include "Material.h"
#include "Object.h"

namespace obj {
  class Voxel: public Base {
  public:
    struct Instance {
      glm::mat4 transform{};
      float     random     = 1.f; //static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
      matId     materialID = 0;
    };

  private:
    matId materialID;

  public:
    Voxel(Transform transform = {}, matId matId = 0): Base{transform}, materialID{matId} {};

    Instance toInstance() const {
      return Instance{
        .transform = transform.mat4(),
        .materialID = materialID
      };
    }
  };
}
