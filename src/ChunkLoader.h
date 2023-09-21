#pragma once
#include <map>

#include "imgui.h"
#include "UIModule.h"
#include "VisualContext.h"

class ChunkLoader{
	struct Chunk{
		std::vector<Voxel> voxels{};
	};

	vc::VisualContext& vc;
	std::map<std::pair<int, int>, Chunk> chunks{};
	int loadDistance = 48;

	static const float CHUNKSIZE;
	static const float VOXELSIZE;
public:
	ChunkLoader(vc::VisualContext& vc) :vc{ vc }{
		UIModule::add([this]()
			{
				ImGui::Text("Chunks: %d", chunks.size());
			});
	};
	bool loadChunk(int x, int z);
	void loadAround(float x, float z);
};

