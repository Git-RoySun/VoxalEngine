#pragma once
#include <string>
#include <vector>

#include "Device.h"
namespace vc {
	struct PipelineFixedStageInfo {
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
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
		Pipeline(Device& device, const std::string& vertPath, const std::string& fragPath, PipelineFixedStageInfo info);
		~Pipeline();
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

		static PipelineFixedStageInfo defaultPipelineInfo(uint32_t width, uint32_t height);

		void bind(VkCommandBuffer commandBuffer);
	};
}

