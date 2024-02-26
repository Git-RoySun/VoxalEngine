#pragma once
#include <chrono>
#include <random>
#include <array>
#include <map>
#include <glm/vec3.hpp>

//Think of this class as a database for objects
//such that they are synchronized across multiple players
//TODO does not quite handle world updatess because no chunks are loaded here (currently only on gpu)
//Apps should not load multiple worlds at the same time
class Chunk;

class World {
  std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();

  std::map<std::tuple<int32_t, int32_t, int32_t>, std::shared_ptr<Chunk>> loadedChunks{};

  int                                seed;
  std::mt19937                       gen;
  std::array<uint8_t, 256>           permutations;
  std::uniform_int_distribution<int> dist{0, 255};

  void generatePermutations();

public:
  World(int seed = std::random_device()());

  std::shared_ptr<Chunk> getChunk(glm::ivec3 pos);

  void update();
};
