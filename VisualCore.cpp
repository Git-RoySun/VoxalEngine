#include "VisualCore.h"
#include <stdexcept>
#include <array>
namespace vc {
	VisualCore::VisualCore() {
		initPipelineLayout();
		initPipeline();
		initCommandBuffer();
	};

	VisualCore::~VisualCore() {
		vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
	};

	void VisualCore::start() {
		while (!window.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}
		vkDeviceWaitIdle(device.device());
	};

	void VisualCore::initPipelineLayout() {
		VkPipelineLayoutCreateInfo layoutInfo = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr,
		};
		if (vkCreatePipelineLayout(device.device(),&layoutInfo,nullptr,&pipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipeline LAYOUT!");
	};

	void VisualCore::initPipeline() {
		auto pipelineConfig = Pipeline::defaultPipelineInfo(swapChain.width(), swapChain.height());
		pipelineConfig.renderPass = swapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		Pipelines.push_back(
			std::make_unique<Pipeline>(device, "shaders/vert.spv", "shaders/frag.spv", pipelineConfig)
		);
	}

	void VisualCore::initCommandBuffer() {
		commandBuffers.resize(swapChain.imageCount());
		VkCommandBufferAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = device.getCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = static_cast<uint32_t>(commandBuffers.size()),
		};

		if (vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers!");

		for (int i = 0; i < commandBuffers.size() ;i++) {
			VkCommandBufferBeginInfo beginInfo = {
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			};

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("Failed to begin recording for command buffer");
			}

			VkClearValue clearValues[2] = {
				{.color = {0.1f, 0.1f, 0.1f, 1.0f}},
				{.depthStencil = {1.0f, 0}}
			};

			VkRenderPassBeginInfo renderPassInfo = {
				.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
				.renderPass = swapChain.getRenderPass(),
				.framebuffer = swapChain.getFrameBuffer(i),
				.renderArea = {
					.offset = {0,0},
					.extent = swapChain.getSwapChainExtent()
				},
				.clearValueCount = static_cast<uint32_t>(2),
				.pClearValues = clearValues
			};

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			Pipelines[0]->bind(commandBuffers[i]);
			vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i])) {
				throw std::runtime_error("Failed to end recording for command buffer");
			}
		}
	};

	void VisualCore::drawFrame() {
		uint32_t imageIndex;
		auto result = swapChain.acquireNextImage(&imageIndex);

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire chain image!");
		}

		result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to present swap chain image!");
		}
	}
}
