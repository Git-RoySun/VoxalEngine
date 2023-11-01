#include "Voxel.h"
namespace obj {
	const std::vector<glm::vec3> Voxel::Vertices = {
	{-0.5f,-0.5f,-0.5f},//left bottom far     0
	{-0.5f,-0.5f,0.5f}, //left bottom close   1
	{-0.5f,0.5f,-0.5f}, //left top far        2
	{-0.5f,0.5f,0.5f},  //left top close      3
	{0.5f,-0.5f,-0.5f}, //right bottom far    4
	{0.5f,-0.5f,0.5f},  //right bottom close  5
	{0.5f,0.5f,-0.5f},  //right top far       6
	{0.5f,0.5f,0.5f},   //right top close     7
	};

	const std::vector<uint32_t> Voxel::Indices = { 0,1,3, 3,2,0, 5,4,6, 6,7,5, 1,5,7, 7,3,1, 4,0,2, 2,6,4, 3,7,6, 6,2,3, 0,4,5, 5,1,0 };

	const uint32_t Voxel::VertexCount = static_cast<uint32_t> (Vertices.size());
	const uint32_t Voxel::IndexCount = static_cast<uint32_t> (Indices.size());

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