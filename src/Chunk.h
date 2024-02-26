#pragma once
#include <array>
#include <vector>

#include "Voxel.h"
#include "World.h"
class Observer;

class Chunk {
  //TODO should be serializable
  //@params chunkSize: width of chunk in blocks
  static int             chunkSize;
  World&                 world;
  glm::ivec3             position;//chunk space position
  std::vector<Observer*> observers{};

  struct Octree {
    std::array<void*, 8> children = {nullptr};
    //no need to store leaf mask as we have rootSize and leaves are size 1
    ~Octree();
  } root;//Stores modified voxels
  struct Leaf {
    matId material;
  };

  /*Between 1 and 64 */
  uint8_t rootSize = 1;

  //procedural generation variables
  bool decorated = false;//could redecorate chunk every time, compute/memory tradeoff
  int  biome     = 0;

  //void notifyObservers(std::vector<> changes);
  //serialize function should check if changed and generated voxels are identical and remove it from generated voxels instead of saving it
public:
  Chunk(World& world, glm::ivec3 pos);
  void addObserver(Observer* o) { observers.emplace_back(o); }
  void removeObserver(Observer* o) { observers.erase(std::ranges::find(observers, o)); }

  //@params material: if given -1 then voxel is removed instead
  void setVoxel(glm::vec3 pos, matId material);
  //todo should work with a vector of voxels as this process can be batched

  Octree* getVoxels(int finalWidth);//generates chunk on the fly
};
