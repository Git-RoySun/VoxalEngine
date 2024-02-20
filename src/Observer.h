#pragma once
#include <deque>
#include <memory>
#include <vector>
#include "Camera.h"
#include "Voxel.h"

class Chunk;
class World;

class Observer {
  World*     world;
  glm::ivec2 currentChunkPos;
  int        renderDistance;
  gm::Camera camera{};

  std::deque<std::deque<std::shared_ptr<Chunk>>> chunks;
  std::deque<std::shared_ptr<Chunk>>             getCol(int x);

protected:
  void move(glm::vec2 pos);

public:
  Observer(World* world, glm::ivec2 pos = {0, 0}, int renderDistance = 0);
  std::vector<obj::Voxel*> getData() const;
  gm::Camera&              getCamera() { return camera; }
  bool                     changed = false;
};
