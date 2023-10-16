#pragma once
#include "RenderSystem.h"
#include "Voxel.h"
#include "Buffer.h"

namespace vc {
	class VoxelRenderer: public RenderSystem {
		std::unique_ptr<Buffer> vertexBuffer;
		std::unique_ptr<Buffer> indexBuffer;
	protected:
		void initPipeline(VkRenderPass renderPass) override;
	public:
		VoxelRenderer(Device& device);
		void renderVoxels(FrameInfo info, int instanceCount);

		void init(VkDescriptorSetLayout setLayout, VkRenderPass renderPass) override;
	};
}

