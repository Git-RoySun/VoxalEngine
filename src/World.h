#pragma once
#include <chrono>
#include <map>

#include "Voxel.h"

class Chunk;

//Think of this class as a database for objects and materials
//Apps should not load multiple worlds at the same time
class World {
  std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();

  std::map<std::pair<int, int>, std::shared_ptr<Chunk>> loadedChunks{};

public:
  static const int CHUNKSIZE = 16;
  World();

  std::shared_ptr<Chunk> getChunk(glm::ivec2 pos);

  void update();
};
