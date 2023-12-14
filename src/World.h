#pragma once
#include "Voxel.h"
#include <chrono>
#include <map>

#include "Buffer.h"

class Chunk;
class Observer;

//Think of this class as a database for objects
class World {
	std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();
	std::map<std::pair<int,int>,std::shared_ptr<Chunk>> loadedChunks{};

public:
	static const int CHUNKSIZE = 16;
	World();

	void update();
	std::shared_ptr<Chunk> getChunk(glm::ivec2 pos);
};

