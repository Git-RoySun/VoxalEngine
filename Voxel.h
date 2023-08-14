#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "Object.h"

#include <vulkan/vulkan_core.h>

class Voxel : public Object {
public:
	struct Instance {
		glm::vec3 position{};
		alignas(16) glm::vec3 scale{ 1.f };
		alignas(16) glm::vec3 rotation{};
		alignas(16) glm::vec3 colour {1.f};
	};

	struct Vertex {
		glm::vec3 position{0.f};
		static constexpr glm::vec3 DIRT { .6f,0.3f,0.f };
		static constexpr glm::vec3 GRASS { 0.25f,1.f,0.01f };

		static std::vector<VkVertexInputBindingDescription> getBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
	};
	Voxel(Instance instance):data{instance}{};
	Instance getInstance() { return data; };
private:
	Instance data;
};

