#include "World.h"
#include "Updatable.h"
#include "Chunk.h"

World::World() {}

void World::update() {
  auto                         now   = std::chrono::steady_clock::now();
  std::chrono::duration<float> delta = now - lastUpdate;
  lastUpdate                         = now;
  Updatable::updateAll(delta.count());
}

std::shared_ptr<Chunk> World::getChunk(glm::ivec2 pos) {
  auto res = loadedChunks.find(std::pair(pos.x, pos.y));
  if(res == loadedChunks.end()) {
    auto ptr = std::make_shared<Chunk>(pos);
    loadedChunks.insert(std::pair<std::pair<int, int>, std::shared_ptr<Chunk>>({pos.x, pos.y}, ptr));
    return ptr;
  } else {
    return res->second;
  }
}
