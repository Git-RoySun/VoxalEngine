#include "Chunk.h"
#include "MaterialBuffer.h"
#include "Voxel.h"
#include "Observer.h"

#define SIZE 16.f

std::shared_ptr<Chunk> Chunk::Builder::build() {
  std::shared_ptr<Chunk> res{};
  return res;
}

Chunk::Chunk(glm::ivec2 pos) : position{pos} {
  voxels.emplace_back(
    obj::Transform{
      glm::vec3(pos.x, 0, pos.y),
    },
    MATERIALS.getMaterial("RUBIDIUM")
  );
}

std::vector<obj::Voxel*> Chunk::getVoxels() {
  std::vector<obj::Voxel*> res{};
  for(auto& v: voxels) {
    res.push_back(&v);
  }
  return res;
}

void Chunk::notifyObservers() {
  for(auto o: observers) {
    o->changed = true;
  }
}
