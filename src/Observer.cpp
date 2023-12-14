#include "Observer.h"
#include "Chunk.h"
#include "World.h"

Observer::Observer(World* world, glm::ivec2 pos, int renderDistance):world{world}, renderDistance{ renderDistance } {
	currentChunkPos = glm::ivec2{ -renderDistance,-renderDistance };
	changed = true;
	for (int x = -renderDistance; x <= renderDistance;x++) {
		std::deque<std::shared_ptr<Chunk>> row{};
		for (int y = -renderDistance; y <= renderDistance; y++) {
			row.emplace_back(world->getChunk({x,y}));
		}
		chunks.emplace_back(row);
	}
}

std::vector<obj::Voxel::Instance> Observer::getData() const {
	std::vector<obj::Voxel::Instance> instances{};
	for(auto col:chunks){
		for(auto chunk :col){
			for(auto& voxel:chunk->getVoxels()){
				instances.push_back(voxel.toInstance());
			}
		}
	}
	return instances;
}

void Observer::move(glm::vec2 pos){
	auto direction = glm::ivec2(pos/static_cast<float>(World::CHUNKSIZE)) - currentChunkPos;
	if(direction.x!=0){
		changed = true;
		for(int i = 1;i<=glm::abs(direction.x)&&i<=renderDistance*2+1;i++){
			if (direction.x > 0) {
				auto out = chunks.front();
				for(auto c:out){
					c->removeObserver(this);
				}
				chunks.pop_front();
				chunks.push_back(getCol(renderDistance + i));
			}
			else {
				auto out = chunks.back();
				for (auto c : out) {
					c->removeObserver(this);
				}
				chunks.pop_back();
				chunks.push_front(getCol(-(renderDistance + i)));
			}
		}
	}
	if (direction.y != 0) {
		changed = true;
		int t = -renderDistance;
		for(auto col:chunks){
			for (int i = 1; i <= glm::abs(direction.y) && i <= renderDistance * 2 + 1; i++) {
				if (direction.y > 0){
					col.back()->removeObserver(this);
					col.pop_back();
					auto c = world->getChunk(currentChunkPos + glm::ivec2{ t,renderDistance + i });
					c->addObserver(this);
					col.push_back(c);
				}
				else {
					col.front()->removeObserver(this);
					col.pop_front();
					auto c = world->getChunk(currentChunkPos + glm::ivec2{ t,-(renderDistance + i) });
					c->addObserver(this);
					col.push_back(c);
				}
			}
			t += 1;
		}
	}

	currentChunkPos = pos;
}

std::deque<std::shared_ptr<Chunk>> Observer::getCol(int x) {
	std::deque<std::shared_ptr<Chunk>> col{};
	for (int t = -renderDistance; t <= renderDistance; t += 1) {
		auto c = world->getChunk(currentChunkPos + glm::ivec2{ x, t });
		c->addObserver(this);
		col.push_back(c);
	}
	return col;
}


