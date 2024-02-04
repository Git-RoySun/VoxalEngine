#include "MaterialBuffer.h"

#include "Graphic.h"

static std::vector<std::pair<std::string, Material>> default_materials = {
  {"WHITE", {{1.f, 1.f, 1.f}}},
  {"BLACK", {{0.f, 0.f, 0.f}}},
  {"RED", {{1.f, 0.f, 0.f}}},
  {"GREEN", {{0.f, 1.f, 0.f}}},
  {"BLUE", {{0.f, 0.f, 1.f}}},
};

namespace gm {
  MaterialBuffer::MaterialBuffer(): DeviceBuffer(
    sizeof(Material),
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
    INT8_MAX
  ) {
    addMaterials(default_materials);
  }

  void MaterialBuffer::addMaterials(std::vector<std::pair<std::string, Material>> materials) {
    int offset = nextMatId * sizeof(Material);
    stagingBuffer->map();
    for(auto& [name, material]: materials) {
      stagingBuffer->writeToIndex(&material, nextMatId);
      nameMap[name] = nextMatId++;
    }
    synchronize();
  }
}
