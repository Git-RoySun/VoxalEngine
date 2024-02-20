#pragma once
#include <memory>
#include <vector>

#include "Voxel.h"
class Observer;

class Chunk {
  //TODO should be serializable
  glm::ivec2              position;
  std::vector<obj::Voxel> voxels{};
  std::vector<Observer*>  observers{};

  void notifyObservers();

public:
  class Builder {
  public:
    std::shared_ptr<Chunk> build();
  };

  struct Octree {
    uint32_t pNext;
    uint8_t  validMask;
    uint8_t  leafMask;
  };

  Chunk(glm::ivec2 pos);
  void addObserver(Observer* o) { observers.emplace_back(o); }
  void removeObserver(Observer* o) { observers.erase(std::ranges::find(observers, o)); }

  std::vector<obj::Voxel*> getVoxels();
};
