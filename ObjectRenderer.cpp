#include "ObjectRenderer.h"
#include <stdexcept>

namespace vc {
	ObjectRenderer::ObjectRenderer(Device& device):device{ device } {};

	ObjectRenderer::~ObjectRenderer() {
		vkDestroyPipelineLayout(device.getVkDevice(), pipelineLayout, nullptr);
	};

	void ObjectRenderer::init(VkDescriptorSetLayout setLayout, VkRenderPass renderPass){
		initPipelineLayout(setLayout);
		initPipeline(renderPass);
	}

	void ObjectRenderer::renderObjects(FrameInfo info, std::vector<Object>& objects, MeshGroup& meshes) {
		pipeline->bind(info.commandBuffer);
		meshes.bind(info.commandBuffer);
		for (auto& obj : objects) {
			PushConstantData push{
				.transform = obj.getTransform(),
			};

			vkCmdPushConstants(
				info.commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PushConstantData),
				&push);

			vkCmdBindDescriptorSets(
				info.commandBuffer,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				pipelineLayout,
				0, 1,
				&info.descriptorSet,
				0, nullptr);

			vkCmdDrawIndexed(info.commandBuffer,static_cast<uint32_t>(obj.getModel().getIndices().size()), 1, obj.getModel().getIndexOffset(), 0, 0);
		}
	}

	void ObjectRenderer::initPipelineLayout(VkDescriptorSetLayout setLayout) {
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

	void ObjectRenderer::initPipeline(VkRenderPass renderPass) {
		PipelineFixedStageInfo configInfo{};
		auto pipelineConfig = Pipeline::defaultPipelineInfo(configInfo);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		pipeline = std::make_unique<Pipeline>(device, "shaders/vert.spv", "shaders/frag.spv", pipelineConfig);
	}

}
