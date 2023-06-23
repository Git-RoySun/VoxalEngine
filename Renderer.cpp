#include "Renderer.h"
#include <stdexcept>
#include <array>
#include <vector>
#include <iostream>

namespace vc {
	Renderer::Renderer(Window& _window, Device& _device) : window{ _window }, device{ _device } {
		initSwapChain();
		initCommandBuffer();
	};

	Renderer::~Renderer() { freeCommandBuffers(); };

	void Renderer::initSwapChain() {
		auto extent = window.getExtent();
		while (extent.width == 0 || extent.height == 0) {
			extent = window.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(device.getVkDevice());
		if (swapChain != nullptr) {
			std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain);
			swapChain = std::make_unique<SwapChain>(device, extent, oldSwapChain);

			if (!oldSwapChain->compareSwapFormat(*swapChain.get())) {
				throw std::runtime_error("Swap chain image format has changed!");
			}
		}
		else
			swapChain = std::make_unique<SwapChain>(device, extent);
	}

	void Renderer::initCommandBuffer() {
		commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
		VkCommandBufferAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = device.getCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = static_cast<uint32_t>(commandBuffers.size()),
		};

		if (vkAllocateCommandBuffers(device.getVkDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers!");
	};

	void Renderer::freeCommandBuffers() {
		vkFreeCommandBuffers(
			device.getVkDevice(),
			device.getCommandPool(),
			static_cast<uint32_t>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	VkCommandBuffer Renderer::startFrame() {
		if (frameStatus == ACTIVE)
			throw std::logic_error("Cannot call startFrame when frame is already active/started!");
		auto result = swapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			initSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("Failed to acquire chain image!");
		}

		frameStatus = ACTIVE;
		auto commandBuffer = getActiveCommandBuffer();
		VkCommandBufferBeginInfo beginInfo = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, };

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("Failed to begin recording for command buffer");
		}

		return commandBuffer;
	}
	void Renderer::endFrame() {
		if (frameStatus == IDLE)
			throw std::logic_error("Cannot call endFrame when frame is not already active/started!");

		auto commandBuffer = getActiveCommandBuffer();
		if (vkEndCommandBuffer(commandBuffer)) {
			throw std::runtime_error("Failed to end recording for command buffer");
		}

		auto result = swapChain->submitCommandBuffers(&commandBuffer, &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasResized()) {
			window.resetResized();
			initSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to create swap chain image!");
		}

		frameStatus = IDLE;
		frameIndex = (frameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
	};
	void Renderer::startRenderPass(VkCommandBuffer commandBuffer) {
		if (frameStatus == IDLE)
			throw std::logic_error("Cannot start render pass when frame is not already active/started!");
		if(commandBuffer != getActiveCommandBuffer())
			throw std::logic_error("Cannot start render pass on command buffer from another frame!");

		VkClearValue clearValues[2] = {//background
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

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{
			.x = 0.0f,
			.y = 0.0f,
			.width = static_cast<float>(swapChain->getSwapChainExtent().width),
			.height = static_cast<float>(swapChain->getSwapChainExtent().height),
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};

		VkRect2D scissor{ {0,0}, swapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	};
	void Renderer::endRenderPass(VkCommandBuffer commandBuffer) {
		if (frameStatus == IDLE)
			throw std::logic_error("Cannot end render pass when frame is not already active/started!");		
		if (commandBuffer != getActiveCommandBuffer())
			throw std::logic_error("Cannot end render pass on command buffer from another frame!");

		vkCmdEndRenderPass(commandBuffer);
	};
}
