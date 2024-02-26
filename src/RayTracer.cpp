#include <array>

#include "RayTracer.h"
#include "Graphic.h"

namespace gm {
  RayTracer::RayTracer() {
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
  }

  void RayTracer::bindInstances(std::vector<obj::Voxel*> voxels) {
    std::array<uint32_t, OCTREE_COUNT / 2> octreeData{};
    std::array<uint32_t, LEAF_COUNT>       leafData{};
    std::array<uint32_t, ATOM_COUNT>       atomData{};
  }

  void RayTracer::render(Frame frameInfo) {
    //todo
  }
}
