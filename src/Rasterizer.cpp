#include "Rasterizer.h"
#include "Observer.h"
#include "SwapChain.h"
#include "Utils.hpp"
#include "Window.h"
#include "MaterialBuffer.h"

static std::vector<obj::Vertex> vertices = {
  {
    {-0.5f, 0.5f, -0.5f},
    {-1.f, 0.f, 0.f}
  },//left bottom far     0
  {
    {-0.5f, 0.5f, 0.5f},
    {0.f, 0.f, -1.f}
  },//left bottom close   1
  {
    {-0.5f, -0.5f, 0.5f},
    {0.f, -1.f, 0.f}
  },//left top close      2
  {
    {-0.5f, -0.5f, -0.5f},
    {}
  },//left top far        3
  {
    {0.5f, 0.5f, -0.5f},
    {1.f, 0.f, 0.f}
  }, //right bottom far    4
  {
    {0.5f, 0.5f, 0.5f},
    {0.f, 1.f, 0.f}
  },//right bottom close  5
  {
    {0.5f, -0.5f, 0.5f},
    {}
  },   //right top close     6
  {
    {0.5f, -0.5f, -0.5f},
    {0.f, 0.f, 1.f}
  },//right top far       7
};

static std::vector<uint32_t> indices = {
  0, 1, 2,
  0, 2, 3,
  4, 7, 6,
  4, 6, 5,
  1, 5, 6,
  1, 6, 2,
  7, 4, 0,
  7, 0, 3,
  2, 6, 7,
  2, 7, 3,
  5, 1, 0,
  5, 0, 4
};

static const uint32_t vertexCount = vertices.size();
static const uint32_t indexCount  = indices.size();

namespace gm {
  std::unique_ptr<Buffer> Rasterizer::vertexBuffer = nullptr;
  std::unique_ptr<Buffer> Rasterizer::indexBuffer  = nullptr;

  Rasterizer::Rasterizer() {
    instanceBuffer[activeIndex] = std::make_unique<Buffer>(
      DEVICE,
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      1
    );

    UBO[0] = std::make_unique<Buffer>(
      DEVICE,
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      sizeof(UniformBuffer),
      1,
      DEVICE.getDeviceProperties().limits.minUniformBufferOffsetAlignment
    );
    UBO[0]->map();

    UBO[1] = std::make_unique<Buffer>(
      DEVICE,
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      sizeof(UniformBuffer),
      1,
      DEVICE.getDeviceProperties().limits.minUniformBufferOffsetAlignment
    );
    UBO[1]->map();

    if(!vertexBuffer || !instanceBuffer[activeIndex])initBuffers();
    initDescriptors();
    initPipelineLayout();
    initPipeline();
  }

  void Rasterizer::initBuffers() {
    Buffer vertexStager{
      DEVICE,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      sizeof(vertices[0]),
      vertexCount,
    };
    vertexStager.map();
    vertexStager.writeToBuffer(vertices.data());
    vertexStager.flush();
    vertexBuffer = std::make_unique<Buffer>(
      DEVICE,
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      sizeof(vertices[0]),
      vertexCount
    );
    vertexStager.transfer(vertexBuffer->getVkBuffer());

    Buffer indexStager{
      DEVICE,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      sizeof(indices[0]),
      indexCount,
    };
    indexStager.map();
    indexStager.writeToBuffer(indices.data());
    indexStager.flush();
    indexBuffer = std::make_unique<Buffer>(DEVICE,
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      sizeof(indices[0]),
      indexCount
    );
    indexStager.transfer(indexBuffer->getVkBuffer());
  }

  void Rasterizer::initDescriptors() {
    descriptorPool = DescriptorPool::Builder(DEVICE)
      .setMaxSets(2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2)
      .addPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2)
      .build();

    setLayout = DescriptorSetLayout::Builder(DEVICE)
      .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
      .addBinding(1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
      .build();

    for(int i = 0; i < 2; ++i) {
      auto uboInfo = UBO[i]->descriptorInfo();
      auto matInfo = gm::MaterialBuffer::getInstance().getLocalBuffer().descriptorInfo();
      DescriptorWriter(*setLayout, *descriptorPool)
        .writeBuffer(0, &uboInfo)
        .writeBuffer(1, &matInfo)
        .build(descriptorSets[i]);
    }
  }

  void Rasterizer::initPipelineLayout() {
    VkPushConstantRange pushConstantRange{
      .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
      .offset = 0,
      .size = sizeof(PushConstant)
    };

    std::vector setLayouts{setLayout->getDescriptorSetLayout()};

    VkPipelineLayoutCreateInfo layoutInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .setLayoutCount = static_cast<uint32_t>(setLayouts.size()),
      .pSetLayouts = setLayouts.data(),
      .pushConstantRangeCount = 1,
      .pPushConstantRanges = &pushConstantRange,
    };

    VK_CHECK_RESULT(vkCreatePipelineLayout(DEVICE.getVkDevice(), &layoutInfo, nullptr, &pipelineLayout), "Failed to create pipeline Layout!")
  };

  void Rasterizer::initPipeline() {
    RasterizationPipeline::FixedStageInfo configInfo{};

    auto pipelineConfig = RasterizationPipeline::defaultPipelineInfo(configInfo);

    pipelineConfig.multisampleInfo.rasterizationSamples = DEVICE.getMsaaSample();
    pipelineConfig.rasterizationInfo.cullMode           = VK_CULL_MODE_BACK_BIT;
    pipelineConfig.renderPass                           = Module::getInstance().getWindow().getSwapChain().getRenderPass();
    pipelineConfig.pipelineLayout                       = pipelineLayout;

    pipeline = std::make_unique<RasterizationPipeline>("shaders/vert.spv", "shaders/frag.spv", pipelineConfig);
  }

  void Rasterizer::bindInstances(std::vector<obj::Voxel::Instance> voxels) {
    instanceCount = voxels.size();
    Buffer stagingBuffer{
      DEVICE,
      VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
      sizeof(voxels[0]),
      static_cast<uint32_t>(voxels.size())
    };

    stagingBuffer.map();
    stagingBuffer.writeToBuffer(voxels.data());
    stagingBuffer.flush();
    instanceBuffer[activeIndex]->invalidate();

    instanceBuffer[activeIndex = !activeIndex] = std::make_unique<Buffer>(
      DEVICE,
      VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
      sizeof(voxels[0]),
      static_cast<uint32_t>(voxels.size())
    );

    stagingBuffer.transfer(instanceBuffer[activeIndex]->getVkBuffer());
  }

  void Rasterizer::render(Frame frameInfo) {
    if(frameInfo.observer->changed) {
      bindInstances(frameInfo.observer->getData());
      frameInfo.observer->changed = false;
    }

    vkCmdBindPipeline(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->getVkPipeline());

    VkBuffer     buffers[] = {vertexBuffer->getVkBuffer(), instanceBuffer[activeIndex]->getVkBuffer()};
    VkDeviceSize offsets[] = {0, 0};
    vkCmdBindVertexBuffers(frameInfo.commandBuffer, 0, 2, buffers, offsets);
    vkCmdBindIndexBuffer(frameInfo.commandBuffer, indexBuffer->getVkBuffer(), 0, VK_INDEX_TYPE_UINT32);

    UniformBuffer data{
      .projection = frameInfo.observer->getCamera().getProjection(),
      .view = frameInfo.observer->getCamera().getView(),
      .light = light,
    };
    UBO[activeIndex]->writeToBuffer(&data);
    UBO[activeIndex]->flush();

    vkCmdBindDescriptorSets(frameInfo.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[activeIndex], 0, nullptr);

    PushConstant pushConstantsData{.time = 0};

    vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pushConstantsData);

    vkCmdDrawIndexed(frameInfo.commandBuffer, indexCount, instanceCount, 0, 0, 0);
  }

  void Rasterizer::cleanup() {
    vkDeviceWaitIdle(DEVICE.getVkDevice());
    vertexBuffer = nullptr;
    indexBuffer  = nullptr;
  }
}
