#include "Voxel.h"

std::vector<VkVertexInputBindingDescription> Voxel::Vertex::getBindingDescription() {
	return {
		{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX},
		{1, sizeof(Instance), VK_VERTEX_INPUT_RATE_INSTANCE},
	};
}

std::vector<VkVertexInputAttributeDescription> Voxel::Vertex::getAttributeDescriptions() {
	return {
		{0,0,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)},
		{1,1,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Instance, position)},
		{2,1,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Instance, scale)},
		{3,1,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Instance, rotation) },
		{4,1,VK_FORMAT_R32_UINT, offsetof(Instance, materialID)},// material index
	};
}
