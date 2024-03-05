#pragma once
#include <deque>
#include <memory>

#include "Camera.h"
#include "Voxel.h"
#include "Chunk.h"

typedef std::deque<std::deque<std::deque<std::shared_ptr<Chunk>>>> ChunkMap;

class Observer {
  World&     world;
  glm::ivec3 position;
  gm::Camera camera{};

  ChunkMap chunks;

  //helper functions for move
  void moveX(int x);
  void moveY(int y);
  void moveZ(int z);

protected:
  void move(glm::vec3 pos);

public:
  Observer(World& world, glm::ivec3 pos);
  void        update(glm::vec3 pos, matId material);
  gm::Camera& getCamera() { return camera; }
  ChunkMap&   getChunks() { return chunks; }
};
