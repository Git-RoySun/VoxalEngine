#pragma once
#include "Object.h"
#include <vector>
#include <glm/vec3.hpp>

#include <vulkan/vulkan_core.h>

namespace obj {
	class Voxel : public Base {
	public:
		struct Instance {
			glm::vec3 position{};
			alignas(16) glm::vec3 scale{ 1.f };
			alignas(16) glm::vec3 rotation{};
			alignas(16) uint32_t materialID = 0;
		};

		static std::vector<VkVertexInputBindingDescription> getBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

		Voxel(Instance instance);
		Instance getInstance();
	private:
		uint32_t materialID;
	};
}
