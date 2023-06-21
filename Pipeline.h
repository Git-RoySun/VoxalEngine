#pragma once
#include <string>
#include <vector>

#include "Device.h"
namespace vc {
	struct PipelineFixedStageInfo {
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class Pipeline {
		Device& device;
		VkPipeline vkPipeline;
		VkShaderModule vertShader;
		VkShaderModule fragShader;

		static std::vector<char> readFile(const std::string&
			filePath);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	public:
		Pipeline() = default;
		Pipeline(Device& device, const std::string& vertPath, const std::string& fragPath, PipelineFixedStageInfo info);
		~Pipeline();

		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

		static PipelineFixedStageInfo defaultPipelineInfo(PipelineFixedStageInfo& configInfo);

		void bind(VkCommandBuffer commandBuffer);
	};
}

