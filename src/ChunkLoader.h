#pragma once
#include <map>

#include "VisualContext.h"

class ChunkLoader{
	struct Chunk{
		std::vector<Voxel> voxels{};
	};

	vc::VisualContext& vc;
	std::map<std::pair<int, int>, Chunk> chunks{};
	int loadDistance = 32;

	static const float CHUNKSIZE;
	static const float VOXELSIZE;
public:
	ChunkLoader(vc::VisualContext& vc) :vc{ vc } {};
	void loadChunk(int x, int z);
	void loadAround(float x, float z);
};

