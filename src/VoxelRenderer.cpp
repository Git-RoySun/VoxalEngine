#include "VoxelRenderer.h"

static std::vector<Voxel::Vertex> vertices = {
	{{-1.f,-1.f,-1.f} },//left bottom far     0
  {{-1.f,-1.f,1.f} }, //left bottom close   1
  {{-1.f,1.f,-1.f} }, //left top far        2
  {{-1.f,1.f,1.f} },  //left top close      3
  {{1.f,-1.f,-1.f} }, //right bottom far    4
  {{1.f,-1.f,1.f} },  //right bottom close  5
  {{1.f,1.f,-1.f} },  //right top far       6
  {{1.f,1.f,1.f} },   //right top close     7
};

static std::vector<uint32_t> indices = {0,1,3, 3,2,0, 5,4,6, 6,7,5, 1,5,7, 7,3,1, 4,0,2, 2,6,4, 3,7,6, 6,2,3, 0,4,5, 5,1,0};

static const uint32_t vertexCount = static_cast<uint32_t> (vertices.size());
static const uint32_t indexCount = static_cast<uint32_t> (indices.size());

namespace vc {
	VoxelRenderer::VoxelRenderer(Device& device):RenderSystem(device){
    vertexBuffer = std::make_unique<Buffer>(
      device,
      sizeof(vertices[0]),
      vertexCount,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    indexBuffer = std::make_unique<Buffer>(
      device,
      sizeof(indices[0]),
      indexCount,
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    Buffer vertexStager{
		device,
		  sizeof(vertices[0]),
      vertexCount,
		  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    };

    Buffer indexStager{
    	device,
      sizeof(indices[0]),
      indexCount,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    };

    vertexStager.map();
    indexStager.map();

    vertexStager.writeToBuffer((void*)vertices.data());
    indexStager.writeToBuffer((void*)indices.data());

    device.copyBuffer(vertexStager.getVkBuffer(), vertexBuffer->getVkBuffer(), sizeof(vertices[0]) *vertices.size());
    device.copyBuffer(indexStager.getVkBuffer(), indexBuffer->getVkBuffer(), sizeof(indices[0]) *indices.size());
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

		vkCmdDrawIndexed(info.commandBuffer, indices.size(), instanceCount, 0, 0, 0);
	
	}

  void VoxelRenderer::initPipeline(VkRenderPass renderPass) {
    PipelineFixedStageInfo configInfo{};
    auto pipelineConfig = Pipeline::defaultPipelineInfo(configInfo);
    pipelineConfig.multisampleInfo.rasterizationSamples = device.getMsaaSample();
    pipelineConfig.rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<Pipeline>(device, "shaders/vert.spv", "shaders/frag.spv", pipelineConfig, Voxel::Vertex::getBindingDescription(), Voxel::Vertex::getAttributeDescriptions());
  }
}
