#pragma once
#include <memory>
#include <vector>


#include "volk.h"
#include "Graphic.h"
#include "Voxel.h"
#include "RasterizationPipeline.h"

namespace gm {
	class Buffer;

	class Rasterizer {
		struct PushConstant {
			glm::mat4 transform;
			uint32_t time;
		};

		static std::unique_ptr<Buffer> vertexBuffer;
		static std::unique_ptr<Buffer> indexBuffer;
		static void initBuffers();

		std::unique_ptr<Buffer> instanceBuffer{};
		std::unique_ptr<Buffer> oldInstanceBuffer{};
		uint32_t instanceCount = 0;

		VkPipelineLayout pipelineLayout;
		std::unique_ptr<RasterizationPipeline> pipeline;

		void initPipelineLayout();
		void initPipeline();
		void bindInstances(std::vector<obj::Voxel::Instance> voxels);

	public:
		Rasterizer();

		void render(Frame frameInfo);
		static void cleanup();
	};
}
