#include "RayTracer.h"
#include "Graphic.h"

namespace gm {
  RayTracer::RayTracer() {
    octrees = std::make_unique<DeviceBuffer>(
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
      sizeof(Octree),
      212898376
    );

    leaves = std::make_unique<DeviceBuffer>(
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
      sizeof(Leaf),
      169508864
    );

    atoms = std::make_unique<DeviceBuffer>(
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
      sizeof(Atom),
      134217728
    );
  }

  void RayTracer::bindInstances(std::vector<obj::Voxel*> voxels) { }

  void RayTracer::render(Frame frameInfo) {
    //todo
  }
}
