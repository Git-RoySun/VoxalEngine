#include "Model.h"
#include <stdexcept>
namespace vc {
	Model::Model(Device& device, const std::vector<Vertex>& vertices) :device { device } {
		initVertexBuffers(vertices);
	}

	Model::~Model() {
		vkDestroyBuffer(device.getVkDevice(), vertexBuffer, nullptr);
		vkFreeMemory(device.getVkDevice(), vertexMemory, nullptr);
	}

	void Model::initVertexBuffers(const std::vector<Vertex>& vertices) {
		vertexCount = static_cast<uint32_t>(vertices.size());
		if (vertexCount < 3) throw std::runtime_error("Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			vertexBuffer,
			vertexMemory);

		
		void* data;//creates space on cpu to mimick gpu memory
		vkMapMemory(device.getVkDevice(), vertexMemory, 0, bufferSize, 0, &data);//synchs both memory
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		//copies vertex data to cpu memory, but since host_choerent_bit, gpu memory is synched to cpu memory, thus vertex data is on gpu
		vkUnmapMemory(device.getVkDevice(), vertexMemory);
	}

	void Model::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}

	void Model::draw(VkCommandBuffer commandBuffer){
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	std::vector<VkVertexInputBindingDescription> Vertex::getBindingDescription() {
		return { {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX} };
	}

	std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions() {
		return { 
			{0,0,VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, position)},
			{1,0,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)}
		};
	}
}