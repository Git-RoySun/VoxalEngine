#include "OutlineRenderer.h"
static std::vector<Voxel::Vertex> vertices = {
  {{-1.01f,-1.01f,-1.01f} },//left bottom far     0
  {{-1.01f,-1.01f,1.01f} }, //left bottom close   1
  {{-1.01f,1.01f,-1.01f} }, //left top far        2
  {{-1.01f,1.01f,1.01f} },  //left top close      3
  {{1.01f,-1.01f,-1.01f} }, //right bottom far    4
  {{1.01f,-1.01f,1.01f} },  //right bottom close  5
  {{1.01f,1.01f,-1.01f} },  //right top far       6
  {{1.01f,1.01f,1.01f} },   //right top close     7
};

static std::vector<uint32_t> indices = { 0,1, 0,2, 0,4, 7,6, 7,5, 7,3, 1,5, 4,5, 2,3, 2,6, 1,3, 6,4};

static const uint32_t vertexCount = static_cast<uint32_t> (vertices.size());
static const uint32_t indexCount = static_cast<uint32_t> (indices.size());

namespace vc {
  OutlineRenderer::OutlineRenderer(Device& device) :RenderSystem(device) {
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

    device.copyBuffer(vertexStager.getVkBuffer(), vertexBuffer->getVkBuffer(), sizeof(vertices[0]) * vertices.size());
    device.copyBuffer(indexStager.getVkBuffer(), indexBuffer->getVkBuffer(), sizeof(indices[0]) * indices.size());
  }

  void OutlineRenderer::renderOutlines(FrameInfo info, int instanceCount) {
    pipeline->bind(info.commandBuffer);

    VkBuffer buffers[] = { vertexBuffer->getVkBuffer(), info.instanceBuffer };
    VkDeviceSize offsets[] = { 0, 0 };

    vkCmdBindVertexBuffers(info.commandBuffer, 0, 2, buffers, offsets);

    vkCmdBindIndexBuffer(info.commandBuffer, indexBuffer->getVkBuffer(), 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(
      info.commandBuffer,
      VK_PIPELINE_BIND_POINT_GRAPHICS,
      pipelineLayout,
      0, 1,
      &info.descriptorSet,
      0, nullptr);

    vkCmdDrawIndexed(info.commandBuffer, indices.size(), instanceCount, 0, 0, 0);
  }

  void OutlineRenderer::initPipeline(VkRenderPass renderPass) {
    PipelineFixedStageInfo configInfo{};
    auto pipelineConfig = Pipeline::defaultPipelineInfo(configInfo);
    pipelineConfig.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    pipelineConfig.rasterizationInfo.lineWidth = 2.f;
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<Pipeline>(device, "shaders/line.spv", "shaders/frag.spv", pipelineConfig, Voxel::Vertex::getBindingDescription(), Voxel::Vertex::getAttributeDescriptions());
  }
}
