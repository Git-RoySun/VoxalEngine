#include "Model.h"
#include <stdexcept>
namespace vc {
	Model::Model(Device& device, const Builder& builder) :device { device } {
		initVertexBuffers(builder.vertices);
		initIndexBuffers(builder.indices);
	}

	Model::~Model() {
		vkDestroyBuffer(device.getVkDevice(), vertexBuffer, nullptr);
		vkFreeMemory(device.getVkDevice(), vertexMemory, nullptr);
		if(indexed){
			vkDestroyBuffer(device.getVkDevice(), indexBuffer, nullptr);
			vkFreeMemory(device.getVkDevice(), indexMemory, nullptr);
		}
	}

	void Model::initVertexBuffers(const std::vector<Vertex>& vertices) {
		vertexCount = static_cast<uint32_t>(vertices.size());
		if (vertexCount < 3) throw std::runtime_error("Vertex count must be at least 3");
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;
		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingMemory);

		
		void* data;//creates space on cpu to mimick gpu memory
		vkMapMemory(device.getVkDevice(), stagingMemory, 0, bufferSize, 0, &data);//synchs both memory
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		//copies vertex data to cpu memory, but since host_choerent_bit, gpu memory is synched to cpu memory, thus vertex data is on gpu
		vkUnmapMemory(device.getVkDevice(), stagingMemory);

		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			vertexBuffer,
			vertexMemory);
		device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
		vkDestroyBuffer(device.getVkDevice(), stagingBuffer, nullptr);
		vkFreeMemory(device.getVkDevice(), stagingMemory, nullptr);
	}

	void Model::initIndexBuffers(const std::vector<uint32_t>& indices) {
		indexCount = static_cast<uint32_t>(indices.size());
		indexed = indexCount > 0;
		if (!indexed) return;
		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;
		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingMemory);


		void* data;//creates space on cpu to mimick gpu memory
		vkMapMemory(device.getVkDevice(), stagingMemory, 0, bufferSize, 0, &data);//synchs both memory
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		//copies vertex data to cpu memory, but since host_choerent_bit, gpu memory is synched to cpu memory, thus vertex data is on gpu
		vkUnmapMemory(device.getVkDevice(), stagingMemory);

		device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			indexBuffer,
			indexMemory);
		device.copyBuffer(stagingBuffer, indexBuffer, bufferSize);
		vkDestroyBuffer(device.getVkDevice(), stagingBuffer, nullptr);
		vkFreeMemory(device.getVkDevice(), stagingMemory, nullptr);
	}

	void Model::draw(VkCommandBuffer commandBuffer){
		if(indexed) {
			vkCmdDrawIndexed(commandBuffer,indexCount,1,0,0,0);
		}
		else
			vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void Model::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		if(indexed){
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		}
	}

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