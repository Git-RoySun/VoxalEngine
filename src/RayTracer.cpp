#include <array>
#include <cmath>

#include "RayTracer.h"
#include "Graphic.h"

static int abs(int x) {
  if(x < 0) return -x;
  else return x + 1;
}

namespace gm {
  RayTracer::RayTracer(Observer* context): Renderer(context) {
    octrees = std::make_unique<DeviceBuffer>(
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
      sizeof(Octree),
      OCTREE_COUNT,
      true
    );

    leaves = std::make_unique<DeviceBuffer>(
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
      sizeof(Leaf),
      LEAF_COUNT,
      true
    );

    atoms = std::make_unique<DeviceBuffer>(
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
      sizeof(Atom),
      ATOM_COUNT,
      true
    );
    bindInstances(context->getChunks());
  }

  void RayTracer::setLeaf(Chunk::Octree* octree) {
    //todo
  }

  void RayTracer::setAtom(Chunk::Leaf* leaf) {
    //todo
  }

  void RayTracer::bindInstances(ChunkMap& chunks) {
    for(int x = 0; x < chunks.size(); x++) {
      for(int y = 0; y < chunks[x].size(); y++) {
        for(int z = 0; z < chunks[x][y].size(); z++) {
          auto chunk       = chunks[x][y][z];
          int  distance    = std::max(abs(x - 16), std::max(abs(y - 16), abs(z - 16)));
          int  detailLevel = pow(2, -(5 - distance));
          auto node        = chunk->getVoxels(detailLevel);

          if(detailLevel == 1) { setAtom(reinterpret_cast<Chunk::Leaf*>(node)); }
          else { setLeaf(node); }
        }
      }
    }
  }

  void RayTracer::render(Frame frameInfo) {
    //todo
  }
}
