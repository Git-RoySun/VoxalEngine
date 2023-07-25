#include "Model.h"
#include <stdexcept>
namespace vc {
	Model::Model(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) : vertices{ vertices }, indices{ indices } {}

	Model::~Model() {}

	std::vector<VkVertexInputBindingDescription> Vertex::getBindingDescription() {
		return { {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX} };
	}

	std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions() {
		return { 
			{0,0,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)},
			{1,0,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)}
		};
	}
}