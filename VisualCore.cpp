#include "VisualCore.h"
#include <stdexcept>
#include <array>
#include <vector>
namespace vc {
	VisualCore::VisualCore() {
		loadModels();
		initPipelineLayout();
		initSwapChain();
		initCommandBuffer();
	};

	VisualCore::~VisualCore() {
		vkDestroyPipelineLayout(device.getVkDevice(), pipelineLayout, nullptr);
	};

	void VisualCore::start() {
		while (!window.shouldClose()) {
			glfwPollEvents();
			drawFrame();
		}
		vkDeviceWaitIdle(device.getVkDevice());
	};


	void VisualCore::loadModels() {
		std::vector<Vertex> vertices {
			{ { 0.0f, -0.5f, 0.0f }, Vertex::RED },
			{ { 0.5f, 0.5f, 0.0f } , Vertex::BLUE },
			{ { -0.5f, 0.5f, 0.0f }, Vertex::GREEN },
		};

		model = std::make_unique<Model>(device, vertices);
	}

	void VisualCore::initPipelineLayout() {
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
		if (vkCreatePipelineLayout(device.getVkDevice(),&layoutInfo,nullptr,&pipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipeline LAYOUT!");
	};

	void VisualCore::initPipeline() {
		PipelineFixedStageInfo configInfo{};
		auto pipelineConfig = Pipeline::defaultPipelineInfo(configInfo);
		pipelineConfig.renderPass = swapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		pipeline = std::make_unique<Pipeline>(device, "shaders/vert.spv", "shaders/frag.spv", pipelineConfig);
	}


	void VisualCore::initSwapChain() {
		auto extent = window.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = window.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(device.getVkDevice());
		if (swapChain!=nullptr) {
			swapChain = std::make_unique<SwapChain>(device, extent, std::move(swapChain));
			if (swapChain->imageCount() != commandBuffers.size()) {
				freeCommandBuffers();
				initCommandBuffer();
			}
		}
		else 
			swapChain = std::make_unique<SwapChain>(device, extent);

		initPipeline();
	}

	void VisualCore::initCommandBuffer() {
		commandBuffers.resize(swapChain->imageCount());
		VkCommandBufferAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = device.getCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = static_cast<uint32_t>(commandBuffers.size()),
		};

		if (vkAllocateCommandBuffers(device.getVkDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers!");
	};

	void VisualCore::recordCommandBuffer(int imageIndex) {
		static int frame = 0;
		frame = (frame + 1) % 1000;

		VkCommandBufferBeginInfo beginInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		};

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin recording for command buffer");
		}

		VkClearValue clearValues[2] = {
			{.color = {0.01f, 0.01f, 0.01f, 1.0f}},
			{.depthStencil = {1.0f, 0}}
		};

		VkRenderPassBeginInfo renderPassInfo = {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = swapChain->getRenderPass(),
			.framebuffer = swapChain->getFrameBuffer(imageIndex),
			.renderArea = {
				.offset = {0,0},
				.extent = swapChain->getSwapChainExtent()
			},
			.clearValueCount = static_cast<uint32_t>(2),
			.pClearValues = clearValues
		};

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(swapChain->getSwapChainExtent().width),
			.height = static_cast<float>(swapChain->getSwapChainExtent().height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};

		VkRect2D scissor{ {0,0}, swapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		pipeline->bind(commandBuffers[imageIndex]);
		model->bind(commandBuffers[imageIndex]);

		for (int i = 0; i < 4; i++) {
			PushConstantData push{
				.offset = {-0.5f+0.0005F*frame, -0.4f + i * 0.25f, 0.0f},
				.color = {0.0f, 0.0f, 0.1f + i * 0.3f},
			};
			vkCmdPushConstants(
				commandBuffers[imageIndex],
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(PushConstantData),
				&push);
			model->draw(commandBuffers[imageIndex]);
		};

		vkCmdEndRenderPass(commandBuffers[imageIndex]);

		if (vkEndCommandBuffer(commandBuffers[imageIndex])) {
			throw std::runtime_error("Failed to end recording for command buffer");
		}
	}

	void VisualCore::freeCommandBuffers() {
		vkFreeCommandBuffers(
			device.getVkDevice(),
			device.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	void VisualCore::drawFrame() {
		uint32_t imageIndex;
		auto result = swapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			initSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire chain image!");
		}

		recordCommandBuffer(imageIndex);
		result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasResized()) {
			window.resetResized();
			return;
		}

		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to create swap chain image!");
		}
	}
}
