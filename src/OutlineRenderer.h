#pragma once
#include "RenderSystem.h"
#include "Voxel.h"
#include "Buffer.h"

namespace vc {
	class OutlineRenderer : public RenderSystem {
		std::unique_ptr<Buffer> vertexBuffer;
		std::unique_ptr<Buffer> indexBuffer;
	public:
		OutlineRenderer(Device& device);
		void renderOutlines(FrameInfo info, int instanceCount);
		void initPipeline(VkRenderPass renderPass) override;
	};
}

