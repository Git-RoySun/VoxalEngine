#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <vector>

#include "Device.h"

namespace vc {
	struct Vertex {
		glm::vec2 position;
		glm::vec3 color;

		static constexpr glm::vec3 RED { 1.0f,0.0f,0.0f };
		static constexpr glm::vec3 GREEN { 0.0f,1.0f,0.0f };
		static constexpr glm::vec3 BLUE { 0.0f,0.0f,1.0f };

		static std::vector<VkVertexInputBindingDescription> getBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};

	struct Transform {
		glm::vec2 translation;
		glm::vec2 scale{1.0f, 1.0f};


		glm::mat2 mat2() {
			return glm::mat2
			{
				{scale.x, 0.0f},
				{ 0.0f, scale.y }
			};
		};
	};

	class Model {
	private:
		Device& device;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexMemory;
		uint32_t vertexCount;

		void initVertexBuffers(const std::vector<Vertex> &vertices);

	public:
		Model(Device& device, const std::vector<Vertex>& vertices);
		~Model();

		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	};
}

