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

  Chunk(glm::ivec2 pos);
  void                           addObserver(Observer* o) { observers.emplace_back(o); }
  void                           removeObserver(Observer* o) { observers.erase(std::ranges::find(observers, o)); }
  const std::vector<obj::Voxel>& getVoxels() const { return voxels; }
};
