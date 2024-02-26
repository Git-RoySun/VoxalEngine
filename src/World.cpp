#include <numeric>

#include "World.h"
#include "Chunk.h"
#include "Updatable.h"

World::World(int seed): seed(seed), gen(seed) {
  generatePermutations();
}

void World::generatePermutations() {
  std::iota(permutations.begin(), permutations.end(), 0);
  for(int i = 0; i < 256; ++i) {
    int j = dist(gen); // Generate random index
    std::swap(permutations[i], permutations[j]);
  }
}

std::shared_ptr<Chunk> World::getChunk(glm::ivec3 pos) {
  std::tuple key{pos.x, pos.y, pos.z};

  auto it = loadedChunks.find(key);
  if(it == loadedChunks.end()) {
    auto chunk        = std::make_shared<Chunk>(*this, pos);
    loadedChunks[key] = chunk;
    return chunk;
  }
  return it->second;
}

void World::update() {
  auto                         now   = std::chrono::steady_clock::now();
  std::chrono::duration<float> delta = now - lastUpdate;
  lastUpdate                         = now;
  Updatable::updateAll(delta.count());
}
