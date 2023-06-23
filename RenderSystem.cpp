#include "RenderSystem.h"
#include <stdexcept>
#include <array>
#include <vector>
#include <iostream>
namespace vc {
	RenderSystem::RenderSystem(Device& device, VkRenderPass renderPass):device{ device } {
		initPipelineLayout();
		initPipeline(renderPass);
	};

	RenderSystem::~RenderSystem() {
		vkDestroyPipelineLayout(device.getVkDevice(), pipelineLayout, nullptr);
	};

	void RenderSystem::renderObjects(VkCommandBuffer commandBuffer, std::vector<Object>& objects) {
		pipeline->bind(commandBuffer);
		for (auto& obj : objects) {
			PushConstantData push{
				.transform = obj.transform.mat4(),
				.offset = obj.transform.translation,
			};

			vkCmdPushConstants(
				commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PushConstantData),
				&push);

			obj.model->bind(commandBuffer);
			obj.model->draw(commandBuffer);

		}
	}

	void RenderSystem::initPipelineLayout() {
		VkPushConstantRange pushConstantRange{
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
			.offset = 0,
			.size = sizeof(PushConstantData)
		};

		VkPipelineLayoutCreateInfo layoutInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 1,
			.pPushConstantRanges = &pushConstantRange,
		};
		if (vkCreatePipelineLayout(device.getVkDevice(), &layoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipeline LAYOUT!");
	};

	void RenderSystem::initPipeline(VkRenderPass renderPass) {
		PipelineFixedStageInfo configInfo{};
		auto pipelineConfig = Pipeline::defaultPipelineInfo(configInfo);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		pipeline = std::make_unique<Pipeline>(device, "shaders/vert.spv", "shaders/frag.spv", pipelineConfig);
	}

}
