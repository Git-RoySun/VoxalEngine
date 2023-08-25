#pragma once
#include "RenderSystem.h"
#include "Voxel.h"
#include "Buffer.h"

namespace vc {
	class VoxelRenderer: public RenderSystem {
		std::unique_ptr<Buffer> vertexBuffer;
		std::unique_ptr<Buffer> indexBuffer;
	public:
		VoxelRenderer(Device& device);
		void renderVoxels(FrameInfo info, int instanceCount);

		void initPipeline(VkRenderPass renderPass) override;
	};
}

