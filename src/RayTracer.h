#pragma once
#include <bitset>

#include "DeviceBuffer.h"
#include "Renderer.h"

#define OCTREE_COUNT 212898376
#define LEAF_COUNT 169508864
#define ATOM_COUNT 134217728

namespace gm {
  class RayTracer: public Renderer {
    struct Octree {//16
      std::bitset<8> validMask = 0;
      std::bitset<8> leafMask  = 0;
    }                root;

    struct Leaf {//32
      std::bitset<8> red       = 0;
      std::bitset<8> green     = 0;
      std::bitset<8> blue      = 0;
      std::bitset<8> validMask = 0;
    };

    struct Atom {//32
      matId    matId = 0;
      uint16_t shade = 0;
    };

    std::unique_ptr<DeviceBuffer> octrees;
    std::unique_ptr<DeviceBuffer> leaves;
    std::unique_ptr<DeviceBuffer> atoms;

    void bindInstances(ChunkMap& chunks) override;
    void setLeaf(Chunk::Octree* octree);
    void setAtom(Chunk::Leaf* leaf);

  public:
    RayTracer(Observer* context);
    void render(Frame frameInfo) override;
  };
}
