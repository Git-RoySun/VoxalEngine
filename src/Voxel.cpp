#include "Voxel.h"
namespace obj {
	Voxel::Voxel(Instance ins) :Base{ {ins.position,ins.rotation,ins.scale} }, materialID{ins.materialID} {}

	Voxel::Instance Voxel::getInstance(){
		return Instance{ transform.position,transform.rotation,transform.scale,materialID };
	}


	std::vector<VkVertexInputBindingDescription> Voxel::getBindingDescription() {
		return {
			{0, sizeof(glm::vec3), VK_VERTEX_INPUT_RATE_VERTEX},
			{1, sizeof(Instance), VK_VERTEX_INPUT_RATE_INSTANCE},
		};
	}

	std::vector<VkVertexInputAttributeDescription> Voxel::getAttributeDescriptions() {
		return {
			{0,0,VK_FORMAT_R32G32B32_SFLOAT, 0},
			{1,1,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Instance, position)},
			{2,1,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Instance, scale)},
			{3,1,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Instance, rotation) },
			{4,1,VK_FORMAT_R32_UINT, offsetof(Instance, materialID)},// material index
		};
	}
}