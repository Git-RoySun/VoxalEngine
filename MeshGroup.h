#pragma once
#include <memory>

#include "Buffer.h"
#include "Model.h"

namespace vc{
	class MeshGroup {
		std::unique_ptr<Buffer> indexBuffer{};
		std::unique_ptr<Buffer> vertexBuffer{};

		int vertices = 0;
		int indices = 0;
	public:
		MeshGroup(Device& device);
		int addMesh(Model& model);
		void bind(VkCommandBuffer commandBuffer);
	};
}

