#include "MeshGroup.h"

#include <iostream>

namespace vc{
	MeshGroup::MeshGroup(Device& device){
		vertexBuffer = std::make_unique<Buffer>(
			device,
			sizeof(Vertex),
			100000,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);
		vertexBuffer->map();

		indexBuffer = std::make_unique<Buffer>(
			device,
			sizeof(uint32_t),
			100000,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);
		indexBuffer->map();
	};

	int MeshGroup::addMesh(Model& model){
		auto res = indices;

		VkDeviceSize offset = static_cast<VkDeviceSize>(vertexBuffer->getAlignmentSize() * vertices);
		VkDeviceSize size = static_cast<VkDeviceSize>(vertexBuffer->getAlignmentSize()*model.getVertices().size());
		vertexBuffer->writeToBuffer(model.getVertices().data(),size, offset);
		vertexBuffer->flush();

		offset = static_cast<VkDeviceSize>(indexBuffer->getAlignmentSize() * indices);
		size = static_cast<VkDeviceSize>(indexBuffer->getAlignmentSize() * model.getIndices().size());
		indexBuffer->writeToBuffer(model.getIndices().data(), size, offset);
		indexBuffer->flush();

		vertices += static_cast<int>(model.getVertices().size());
		indices += static_cast<int>(model.getIndices().size());

		return res;
	}

	void MeshGroup::bind(VkCommandBuffer commandBuffer){
		VkBuffer buffers[] = { vertexBuffer->getVkBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getVkBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}

}
