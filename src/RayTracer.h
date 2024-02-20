#pragma once
#include "DeviceBuffer.h"
#include "Renderer.h"

namespace gm {
  class RayTracer: public Renderer {
    struct Octree {//16
      uint8_t validMask;
      uint8_t leafMask;
    }         root;

    struct Leaf {//32
      uint8_t red;
      uint8_t green;
      uint8_t blue;
      uint8_t validMask;
    };

    struct Atom {//32
      matId    matId;
      uint16_t shade;
    };

    std::unique_ptr<DeviceBuffer> octrees;
    std::unique_ptr<DeviceBuffer> leaves;
    std::unique_ptr<DeviceBuffer> atoms;

    void bindInstances(std::vector<obj::Voxel*> voxels);

  public:
    RayTracer();
    void render(Frame frameInfo) override;
  };
}
