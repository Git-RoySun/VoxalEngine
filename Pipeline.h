#pragma once
#include <string>
#include <vector>

#include "Device.h"
namespace vc {
	struct PipelineStageInfo {};

	class Pipeline {
		Device& device;
		VkPipeline vkPipeline;
		VkShaderModule vertShader;
		VkShaderModule fragShader;

		static std::vector<char> readFile(const std::string&
			filePath);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	public:
		Pipeline(Device& device, const std::string& vertPath, const std::string& fragPath, PipelineStageInfo info);

		~Pipeline(){}
		Pipeline(const Pipeline&) = delete;
		Pipeline& operator=(const Pipeline&) = delete;

		static PipelineStageInfo defaultPipelineInfo(uint32_t height, uint32_t width);

	};
}

