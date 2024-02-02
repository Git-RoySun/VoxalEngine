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
  MaterialBuffer::MaterialBuffer(): Buffer(
    DEVICE,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
    sizeof(Material),
    INT8_MAX
  ) {
    addMaterials(default_materials);
  }

  void MaterialBuffer::addMaterials(std::vector<std::pair<std::string, Material>> materials) {
    int    offset = nextMatId * sizeof(Material);
    Buffer stagingBuffer{
      DEVICE,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      sizeof(Material),
      static_cast<uint32_t>(materials.size())
    };
    stagingBuffer.map();
    for(auto& [name, material]: materials) {
      stagingBuffer.writeToIndex(&material, nextMatId);
      nameMap[name] = nextMatId++;
    }
    stagingBuffer.flush();
    stagingBuffer.transfer(this->getVkBuffer(), offset);
  }
}
