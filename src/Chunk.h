#pragma once
#include <array>
#include <vector>

#include "Observable.h"
#include "Voxel.h"
#include "World.h"

class Observer;

class Chunk: Observable<Observer*> {
  //TODO should be serializable
public:
  struct Octree {
    std::array<void*, 8> children = {nullptr};
    //no need to store leaf mask as we have rootSize and leaves are size 1
    ~Octree();
  };

  struct Leaf {
    matId    material;
    uint16_t shade;
  };

private:
  //@params chunkSize: width of chunk in blocks
  static int             chunkSize;
  World&                 world;
  glm::ivec3             position;//chunk space position
  std::vector<Observer*> observers{};

  Octree root;//Stores modified voxels
  /*Between 1 and 64 */
  uint8_t rootSize = 1;

  //procedural generation variables
  int biome = 0;

  void notifyObservers(glm::vec3 pos, matId material);
  //serialize function should check if changed and generated voxels are identical and remove it from generated voxels instead of saving it
public:
  Chunk(World& world, glm::ivec3 pos);

  //@params material: if given -1 then voxel is removed instead
  void setVoxel(glm::vec3 pos, matId material);
  //todo should work with a vector of voxels as this process can be batched (same with the notifyObservers function)

  Octree* getVoxels(int finalWidth);//generates chunk on the fly
};
