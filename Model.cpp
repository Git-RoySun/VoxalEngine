#include "Model.h"
#include <stdexcept>
namespace vc {
	Model::Model(Device& device, const Builder& builder) :device { device } {
		initVertexBuffers(builder.vertices);
		initIndexBuffers(builder.indices);
	}

	Model::~Model() {}

	void Model::initVertexBuffers(const std::vector<Vertex>& vertices) {
		vertexCount = static_cast<uint32_t>(vertices.size());
		if (vertexCount < 3) throw std::runtime_error("Vertex count must be at least 3");

		uint32_t vertexSize = sizeof(vertices[0]);
		VkDeviceSize bufferSize = vertexSize * vertexCount;

		Buffer stagingBuffer{
			device,
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};
		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)vertices.data());

		vertexBuffer = std::make_unique<Buffer>(
			device,
			vertexSize,
			vertexCount,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT

		);
		device.copyBuffer(stagingBuffer.getVkBuffer(), vertexBuffer->getVkBuffer(), bufferSize);
	}

	void Model::initIndexBuffers(const std::vector<uint32_t>& indices) {
		indexCount = static_cast<uint32_t>(indices.size());
		indexed = indexCount > 0;

		if (!indexed) {
			return;
		}

		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;
		uint32_t indexSize = sizeof(indices[0]);

		Buffer stagingBuffer{
				device,
				indexSize,
				indexCount,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		};

		stagingBuffer.map();
		stagingBuffer.writeToBuffer((void*)indices.data());

		indexBuffer = std::make_unique<Buffer>(
			device,
			indexSize,
			indexCount,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		device.copyBuffer(stagingBuffer.getVkBuffer(), indexBuffer->getVkBuffer(), bufferSize);
	}

	void Model::draw(VkCommandBuffer commandBuffer){
		if (indexed) {
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		}
		else
			vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	void Model::bind(VkCommandBuffer commandBuffer) {
		VkBuffer buffers[] = { vertexBuffer->getVkBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		if(indexed){
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
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