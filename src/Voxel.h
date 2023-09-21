#pragma once
#include <memory>
#include <vector>
#include <glm/vec3.hpp>

#include <vulkan/vulkan_core.h>

class Voxel{
public:
	struct Instance {
		glm::vec3 position{};
		alignas(16) glm::vec3 scale{ 1.f };
		alignas(16) glm::vec3 rotation{};
		alignas(16) uint32_t materialID = 0;
	};

	struct Vertex {
		glm::vec3 position{};
		static std::vector<VkVertexInputBindingDescription> getBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};

	Voxel(Instance instance):data{instance}{};
	Instance getInstance() { return data; };
private:
	Instance data;
};

