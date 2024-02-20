#include "MaterialBuffer.h"

#include "Graphic.h"

static std::vector<std::pair<std::string, Material>> default_materials = {
  {"WHITE", {{255, 255, 255}}},
  {"BLACK", {{0, 0, 0}}},
  {"RED", {{255, 0, 0}}},
  {"GREEN", {{0, 255, 0}}},
  {"BLUE", {{0, 0, 255}}},
  {"BROWN", {{150, 70, 0}}},
  {"GRAY", {{125, 125, 125}}},
  //AI-Generated colors
  {"CYAN", {{0, 255, 255}}},
  {"MAGENTA", {{255, 0, 255}}},
  {"YELLOW", {{255, 255, 0}}},
  {"ORANGE", {{255, 165, 0}}},
  {"PURPLE", {{128, 0, 128}}},
  {"PINK", {{255, 192, 203}}},
  {"GOLD", {{255, 215, 0}}},
  {"SILVER", {{192, 192, 192}}},
  {"BRONZE", {{205, 127, 50}}},
  {"PLATINUM", {{229, 228, 226}}},
  {"COPPER", {{184, 115, 51}}},
  {"TIN", {{166, 166, 166}}},
  {"LEAD", {{119, 136, 153}}},
  {"IRON", {{92, 92, 92}}},
  {"STEEL", {{79, 79, 79}}},
  {"ALUMINUM", {{169, 169, 169}}},
  {"NICKEL", {{192, 192, 192}}},
  {"TITANIUM", {{191, 194, 199}}},
  {"ZINC", {{165, 165, 165}}},
  {"MERCURY", {{150, 150, 150}}},
  {"LEAD", {{119, 136, 153}}},
  {"URANIUM", {{0, 255, 0}}},
  {"PLUTONIUM", {{0, 0, 255}}},
  {"NEPTUNIUM", {{0, 255, 255}}},
  {"THORIUM", {{255, 0, 255}}},
  {"RADIUM", {{255, 255, 0}}},
  {"POLONIUM", {{255, 165, 0}}},
  {"ASTATINE", {{128, 0, 128}}},
  {"FRANCIUM", {{255, 192, 203}}},
  {"RUBIDIUM", {{255, 215, 0}}},
  {"STRONTIUM", {{192, 192, 192}}},
  {"YTTRIUM", {{205, 127, 50}}},
  {"ZIRCONIUM", {{229, 228, 226}}},
};

static uint32_t combine(glm::u8vec3 v) {
  uint32_t result = 0;

  // Combine 'a', 'b', and 'c' into the result using bitwise shifting and OR operations
  result |= static_cast<uint32_t>(v.x) << 24; // Place 'a' at the leftmost side of the 32-bit integer
  result |= static_cast<uint32_t>(v.y) << 16; // Place 'b' after 'a' using 8 bits of offset
  result |= static_cast<uint32_t>(v.z) << 8;  // Place 'c' after 'b' using 16 bits of offset

  return result;
}

namespace gm {
  MaterialBuffer::MaterialBuffer(): DeviceBuffer(
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
    sizeof(uint32_t),
    std::numeric_limits<matId>::max()
  ) {
    addMaterials(default_materials);
  }

  void MaterialBuffer::addMaterials(std::vector<std::pair<std::string, Material>> materials) {
    void*       deviceData = getData();
    uint32_t*   uintPtr    = static_cast<uint32_t*>(deviceData);
    std::vector data(uintPtr, uintPtr + nextMatId);

    for(auto& [name, material]: materials) {
      uint32_t color = combine(material.colour);
      data.push_back(color);
      nameMap[name] = nextMatId++;
    }

    writeData(data.data(), 0, data.size() * sizeof(uint32_t));
  }
}
