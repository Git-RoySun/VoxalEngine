#include "RenderSystem.h"
#include <stdexcept>

namespace vc {
	RenderSystem::RenderSystem(Device& device) :device{ device } {};

	RenderSystem::~RenderSystem() {
		vkDestroyPipelineLayout(device.getVkDevice(), pipelineLayout, nullptr);
	};

	void RenderSystem::init(VkDescriptorSetLayout setLayout, VkRenderPass renderPass) {
		initPipelineLayout(setLayout);
		initPipeline(renderPass);
	}

	void RenderSystem::initPipelineLayout(VkDescriptorSetLayout setLayout) {
		VkPushConstantRange pushConstantRange{
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			.offset = 0,
			.size = sizeof(PushConstantData)
		};

		std::vector<VkDescriptorSetLayout> setLayouts {setLayout};

		VkPipelineLayoutCreateInfo layoutInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = static_cast<uint32_t>(setLayouts.size()),
			.pSetLayouts = setLayouts.data(),
			.pushConstantRangeCount = 1,
			.pPushConstantRanges = &pushConstantRange,
		};
		if (vkCreatePipelineLayout(device.getVkDevice(), &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipeline LAYOUT!");
	};

}
