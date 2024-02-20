#pragma once
#include "DeviceBuffer.h"
#include "Renderer.h"

#define OCTREE_COUNT 212898376
#define LEAF_COUNT 169508864
#define ATOM_COUNT 134217728

namespace gm {
  class RayTracer: public Renderer {
    struct Octree {//16
      uint8_t validMask = 0;
      uint8_t leafMask  = 0;
    }         root;

    struct Leaf {//32
      uint8_t red       = 0;
      uint8_t green     = 0;
      uint8_t blue      = 0;
      uint8_t validMask = 0;
    };

    struct Atom {//32
      matId    matId = 0;
      uint16_t shade = 0;
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
