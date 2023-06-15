#include "Pipeline.h"
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace vc{
	std::vector<char> Pipeline::readFile(const std::string& filePath) {
		std::ifstream file{filePath, std::ios::ate | std::ios::binary};
		//^ this line will call abort (crash) if file is invalid and not throw an error
		if (!file.is_open()) {
			throw std::runtime_error("Pipeline failed to open file "+ filePath);
		}
		size_t fileSize = static_cast<size_t>(file.tellg());
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		return buffer;
	}

	Pipeline::Pipeline(Device& device, const std::string& vertPath, const std::string& fragPath, PipelineStageInfo info) :device{ device }{
		auto vertFile = readFile(vertPath);
		auto fragFile = readFile(fragPath);
		std::cout<<"Vertex Shader Size"<< vertFile.size()<<std::endl;
		std::cout<<"Fragment Shader Size"<< fragFile.size()<<std::endl;
	}
	
	void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
		VkShaderModuleCreateInfo createInfo{
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = code.size(),
			.pCode = reinterpret_cast<const uint32_t*>(code.data()),
		};

		if(vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("Could not create shader module!");
		}
	}

	PipelineStageInfo Pipeline::defaultPipelineInfo(uint32_t height, uint32_t width) {
		PipelineStageInfo configInfo{};

		return configInfo;
	}
}