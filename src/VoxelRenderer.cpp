#include "VoxelRenderer.h"

namespace vc {
	VoxelRenderer::VoxelRenderer(Device& device):RenderSystem(device){}
  void VoxelRenderer::init(VkDescriptorSetLayout setLayout, VkRenderPass renderPass){
    vertexBuffer = std::make_unique<Buffer>(
      device,
      sizeof(obj::Voxel::Vertices[0]),
      obj::Voxel::VertexCount,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    indexBuffer = std::make_unique<Buffer>(
      device,
      sizeof(obj::Voxel::Indices[0]),
      obj::Voxel::IndexCount,
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    Buffer vertexStager{
    device,
      sizeof(obj::Voxel::Vertices[0]),
      obj::Voxel::VertexCount,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    };

    Buffer indexStager{
      device,
      sizeof(obj::Voxel::Indices[0]),
      obj::Voxel::IndexCount,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    };

    vertexStager.map();
    indexStager.map();

    vertexStager.writeToBuffer((void*)obj::Voxel::Vertices.data());
    indexStager.writeToBuffer((void*)obj::Voxel::Indices.data());

    device.copyBuffer(vertexStager.getVkBuffer(), vertexBuffer->getVkBuffer(), sizeof(obj::Voxel::Vertices[0]) * obj::Voxel::Vertices.size());
    device.copyBuffer(indexStager.getVkBuffer(), indexBuffer->getVkBuffer(), sizeof(obj::Voxel::Indices[0]) * obj::Voxel::Indices.size());

    RenderSystem::init(setLayout, renderPass);
	}

	void VoxelRenderer::renderVoxels(FrameInfo info, int instanceCount) {
		pipeline->bind(info.commandBuffer);

    VkBuffer buffers[] = { vertexBuffer->getVkBuffer(), info.instanceBuffer };
    VkDeviceSize offsets[] = { 0, 0 };

    vkCmdBindVertexBuffers(info.commandBuffer, 0, 2, buffers, offsets);

    vkCmdBindIndexBuffer(info.commandBuffer,indexBuffer->getVkBuffer(),0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(
			info.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0, 1,
			&info.descriptorSet,
			0, nullptr);

		vkCmdDrawIndexed(info.commandBuffer, obj::Voxel::Indices.size(), instanceCount, 0, 0, 0);
	}

  void VoxelRenderer::initPipeline(VkRenderPass renderPass) {
    PipelineFixedStageInfo configInfo{};
    auto pipelineConfig = Pipeline::defaultPipelineInfo(configInfo);
    pipelineConfig.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<Pipeline>(device, "shaders/vert.spv", "shaders/frag.spv", pipelineConfig, obj::Voxel::getBindingDescription(), obj::Voxel::getAttributeDescriptions());
  }
}
