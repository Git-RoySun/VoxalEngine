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
  for(int x = 0; x < SIZE; x++) {
    for(int y = 0; y < SIZE; y++) {
      voxels.emplace_back(
        obj::Transform{
          glm::vec3(pos.x + x / SIZE, pos.y + y / SIZE, 0) * 1.2f,
          1 / SIZE
        },M_GREEN);
    }
  }
}

void Chunk::notifyObservers() {
  for(auto o: observers) {
    o->changed = true;
  }
}
