#pragma once
#include <string>
#include <vector>
#include "volk.h"

namespace gm {
  class RasterizationPipeline {
    static std::vector<VkVertexInputBindingDescription>   bindings;
    static std::vector<VkVertexInputAttributeDescription> attributes;

    VkPipeline     vkPipeline;
    VkShaderModule vertShader;
    VkShaderModule fragShader;

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

  public:
    struct FixedStageInfo {
      VkPipelineViewportStateCreateInfo      viewportInfo;
      VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
      VkPipelineRasterizationStateCreateInfo rasterizationInfo;
      VkPipelineMultisampleStateCreateInfo   multisampleInfo;
      VkPipelineColorBlendAttachmentState    colorBlendAttachment;
      VkPipelineColorBlendStateCreateInfo    colorBlendInfo;
      VkPipelineDepthStencilStateCreateInfo  depthStencilInfo;
      std::vector<VkDynamicState>            dynamicStateEnables;
      VkPipelineDynamicStateCreateInfo       dynamicStateInfo;
      VkPipelineLayout                       pipelineLayout = nullptr;
      VkRenderPass                           renderPass     = nullptr;
      uint32_t                               subpass        = 0;
    };

    RasterizationPipeline(const std::string& vertPath, const std::string& fragPath, FixedStageInfo info);
    ~RasterizationPipeline();

    RasterizationPipeline(const RasterizationPipeline&)            = delete;
    RasterizationPipeline& operator=(const RasterizationPipeline&) = delete;

    static FixedStageInfo defaultPipelineInfo(FixedStageInfo& configInfo);
    VkPipeline            getVkPipeline() { return vkPipeline; }
  };
}
