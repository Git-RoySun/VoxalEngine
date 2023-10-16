#pragma once
#include <memory>

#include "Camera.h"
#include "Pipeline.h"
#include "Object.h"
#include "Device.h"


namespace vc {

	struct PushConstantData {
		glm::mat4 transform{1.0f};
	};

	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		VkBuffer instanceBuffer;
		Camera& camera;
		VkDescriptorSet descriptorSet;
	};

	class RenderSystem {
	protected:
		Device& device;
		VkPipelineLayout pipelineLayout;
		std::unique_ptr<Pipeline> pipeline;

		virtual void initPipelineLayout(VkDescriptorSetLayout setLayout);
		virtual void initPipeline(VkRenderPass renderPass) = 0;

	public:
		RenderSystem(Device& device);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;

		virtual void init(VkDescriptorSetLayout setLayout, VkRenderPass renderPass);
	};
}

