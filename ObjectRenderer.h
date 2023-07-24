#pragma once
#include <memory>
#include <vector>

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
		Camera& camera;
		VkDescriptorSet descriptorSet;
	};

	class ObjectRenderer {
		Device& device;
		VkPipelineLayout pipelineLayout;
		std::unique_ptr<Pipeline> pipeline;

		void initPipelineLayout(VkDescriptorSetLayout setLayout);
		void initPipeline(VkRenderPass renderPass);

	public:
		ObjectRenderer(Device& device);
		~ObjectRenderer();

		ObjectRenderer(const ObjectRenderer&) = delete;
		ObjectRenderer& operator=(const ObjectRenderer&) = delete;

		void init(VkDescriptorSetLayout setLayout, VkRenderPass renderPass);
		void renderObjects(FrameInfo info, std::vector<Object>& objects);
	};
}

