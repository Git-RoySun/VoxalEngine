#include "Observer.h"
#include "Chunk.h"
#include "World.h"

Observer::Observer(World& world, glm::ivec3 pos) : world{world}, position{pos}, chunks(32, std::deque(32, std::deque<std::shared_ptr<Chunk>>(32))) {
  for(int x = 0; x < chunks.size(); x++) {
    for(int y = 0; y < chunks[x].size(); y++) {
      for(int z = 0; z < chunks[x][y].size(); z++) {
        chunks[x][y][z] = world.getChunk(glm::ivec3{position.x + x - 16, position.y + y - 16, position.z + z - 16});
      }
    }
  }
}

void Observer::moveX(int x) {
  bool positive = x > 0;

  auto layer = std::deque(32, std::deque<std::shared_ptr<Chunk>>(32));
  for(int y = -16; y < 16; y++) {
    for(int z = -16; z < 16; z++) {
      layer[y + 16][z + 16] = world.getChunk(glm::ivec3{position.x + ((positive) ? 16 : -17), position.y + y, position.z + z});
    }
  }

  if(positive) {
    chunks.pop_front();
    chunks.push_back(layer);
  }
  else {
    chunks.pop_back();
    chunks.push_front(layer);
  }
}

void Observer::moveY(int y) {
  bool positive = y > 0;
  for(int x = 0; x < chunks.size(); x++) {
    //populate layer with correct chunks
    auto layer = std::deque<std::shared_ptr<Chunk>>(32);
    for(int z = -16; z < 16; z++) {
      layer[z + 16] = world.getChunk(glm::ivec3(position.x + x - 16, position.y + ((positive) ? 16 : -17), position.z + z));
    }

    if(positive) {
      chunks[x].pop_front();
      chunks[x].push_back(layer);
    }
    else {
      chunks[x].pop_back();
      chunks[x].push_front(layer);
    }
  }
}

void Observer::moveZ(int z) {
  bool positive = z > 0;

  for(int x = 0; x < chunks.size(); x++) {
    for(int y = 0; y < chunks[x].size(); y++) {
      auto layer = world.getChunk(glm::ivec3(position.x + x - 16, position.y + y - 16, position.z + ((positive) ? 16 : -17)));

      if(positive) {
        chunks[x][y].pop_front();
        chunks[x][y].push_back(layer);
      }
      else {
        chunks[x][y].pop_back();
        chunks[x][y].push_front(layer);
      }
    }
  }
}

void Observer::move(glm::vec3 pos) {
  glm::ivec3 newPos = pos / 8.f;
  if(position != newPos) {
    auto diff = newPos - position;
    if(diff.x)moveX(diff.x);
    if(diff.y)moveY(diff.y);
    if(diff.z)moveZ(diff.z);
    position = newPos;
  }
}
