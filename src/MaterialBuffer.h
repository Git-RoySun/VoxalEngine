#pragma once
#include <map>
#include <string>

#include "Buffer.h"
#include "Material.h"

#define MATERIALS gm::MaterialBuffer::getInstance()
#define M_WHITE MATERIALS.getMaterial("WHITE")
#define M_BLACK MATERIALS.getMaterial("BLACK")
#define M_RED   MATERIALS.getMaterial("RED")
#define M_GREEN MATERIALS.getMaterial("GREEN")
#define M_BLUE  MATERIALS.getMaterial("BLUE")

namespace gm {
  class MaterialBuffer: public Buffer {
    MaterialBuffer();
    std::map<std::string, matId> nameMap;

    uint8_t nextMatId = 0;

  public:
    static MaterialBuffer& getInstance() {
      static MaterialBuffer singleton;
      return singleton;
    }

    void addMaterials(std::vector<std::pair<std::string, Material>> materials);

    matId getMaterial(std::string name) { return nameMap[name]; }//todo add error handling
  };
}
