#pragma once
#include <memory>
#include <vector>

#include "volk.h"
#include "Voxel.h"
#include "RasterizationPipeline.h"
#include "Buffer.h"
#include "Descriptor.h"
#include "Renderer.h"

namespace gm {
  class Rasterizer: public Renderer {
    struct PushConstant {
      uint32_t time;
    };

    struct UniformBuffer {
      glm::mat4 projection;
      glm::mat4 view;
      glm::vec3 light;
    };

    static std::unique_ptr<Buffer> vertexBuffer;
    static std::unique_ptr<Buffer> indexBuffer;

    //todo currently not very efficient with removing/adding single voxels
    std::unique_ptr<Buffer> instanceBuffer[2];
    uint32_t                instanceCount = 0;

    std::unique_ptr<Buffer> UBO[2];

    VkPipelineLayout                       pipelineLayout;
    std::unique_ptr<RasterizationPipeline> pipeline;

    std::unique_ptr<DescriptorPool>      descriptorPool;
    std::unique_ptr<DescriptorSetLayout> setLayout;
    VkDescriptorSet                      descriptorSets[2];

    bool activeIndex = 0;

    void initBuffers();
    void initDescriptors();
    void initPipelineLayout();
    void initPipeline();
    void bindInstances(std::vector<obj::Voxel::Instance> voxels);

  public:
    glm::vec3 light = {2.f, -2.f, 1.f};
    Rasterizer();

    void render(Frame frameInfo) override;

    static void cleanup();//TODO I do not like this, should be a simple desctructor
  };
}
