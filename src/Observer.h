#pragma once
#include <deque>
#include <memory>
#include <vector>
#include <glm/vec2.hpp>

#include "Camera.h"
#include "Dynamic.h"
#include "Voxel.h"

class Chunk;
class World;

class Observer {
	World* world;
	glm::ivec2 currentChunkPos;
	int renderDistance;
	gm::Camera camera{};

	std::deque<std::deque<std::shared_ptr<Chunk>>> chunks;
	std::deque<std::shared_ptr<Chunk>> getCol(int x);

protected:
	void move(glm::vec2 pos);

public:
	Observer(World* world, glm::ivec2 pos = {0, 0}, int renderDistance = 1);
	std::vector<obj::Voxel::Instance> getData() const;
	gm::Camera& getCamera() { return camera; }
	bool changed = false;
};
