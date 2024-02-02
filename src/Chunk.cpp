#include "Chunk.h"
#include "MaterialBuffer.h"
#include "Voxel.h"
#include "Observer.h"

std::shared_ptr<Chunk> Chunk::Builder::build() {
  std::shared_ptr<Chunk> res{};
  return res;
}

Chunk::Chunk(glm::ivec2 pos) : position{pos} {
  obj::Transform transform{glm::vec3(pos.x, 0, pos.y) * 16.f};
  voxels.emplace_back(transform, M_WHITE);
}

void Chunk::notifyObservers() {
  for(auto o: observers) {
    o->changed = true;
  }
}
