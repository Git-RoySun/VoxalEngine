#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "Device.h"

namespace vc {
	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;

		static constexpr glm::vec3 RED { 1.0f, 0.0f, 0.0f };
		static constexpr glm::vec3 GREEN { 0.0f, 1.0f, 0.0f };
		static constexpr glm::vec3 BLUE { 0.0f, 0.0f, 1.0f };

		static std::vector<VkVertexInputBindingDescription> getBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};

	class Model {
	private:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		int indexOffset = 0;

	public:
		Model(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;
		
		std::vector<Vertex>& getVertices() { return vertices; };
		std::vector<uint32_t>& getIndices() { return indices; };
		int getIndexOffset() const { return indexOffset; }
		void setIndexOffset(int offset) { indexOffset = offset; };
	};
}

