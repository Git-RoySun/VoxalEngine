#include <stack>
#include <bitset>
#include "Chunk.h"

static uint32_t abs_svo(int32_t n) {
  return n < 0 ? (-n - 1) : n;
}

static bool outside(glm::ivec3 pos, uint32_t d) {
  return (abs_svo(pos.x) | abs_svo(pos.y) | abs_svo(pos.z)) >= d;
}

static std::bitset<3> getRelativePosition(glm::vec3 pos, glm::vec3 center) {
  std::bitset<3> octant;
  octant[0] = pos.x >= center.x;
  octant[1] = pos.y >= center.y;
  octant[2] = pos.z >= center.z;
  return octant;
}

int Chunk::chunkSize = 8;

Chunk::Chunk(World& world, glm::ivec3 pos) : world{world}, position{pos} {}

Chunk::Octree::~Octree() {
  for(auto child: children) {
    if(child) {
      delete child;
    }
  }
}

void Chunk::setVoxel(glm::vec3 pos, matId material) {
  //Atom space positions
  glm::ivec3 center        = (glm::vec3(position * chunkSize) + glm::vec3(chunkSize / 2)) * 16.f;
  glm::ivec3 normalizedPos = glm::ivec3(pos * 16.f) - center;

  //extend octree if needed
  while(outside(normalizedPos, rootSize)) {
    for(size_t i = 0; i < 8; ++i) {
      if(root.children[i]) {
        auto parent                  = new Octree();
        parent->children[~i & 0b111] = root.children[i];
        root.children[i]             = parent;
      }
    }
    rootSize *= 2;
  }
  //traverse tree and set voxel
  Octree*    node        = &root;
  int        currentSize = rootSize;
  glm::ivec3 currentPos  = center;
  while(currentSize > 1) {
    //while size > 1 node is guaranteed to have not be a leaf
    auto     relPos = getRelativePosition(normalizedPos, currentPos);
    uint32_t index  = relPos.to_ulong();
    if(!node->children[index]) {
      node->children[index] = new Octree();
    }
    node = reinterpret_cast<Octree*>(node->children[index]);
    currentSize /= 2;
    currentPos += (glm::ivec3{relPos[0], relPos[1], relPos[2]} * 2 - glm::ivec3{1}) * currentSize;
  }

  node->children[getRelativePosition(pos, currentPos).to_ulong()] = new Leaf{.material = material};
}

Chunk::Octree* Chunk::getVoxels(int finalWidth) {
  //todo chunk generation rules:
  //Chunk Generation rules:
  //Nothing above perlin heightmap
  //Block type determine by biome
  //Biome determine by noise

  return &root;
}
