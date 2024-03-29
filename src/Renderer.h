#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <cassert>
#include <memory>
#include <vector>

#include "Window.h"
#include "SwapChain.h"


namespace vc {
	enum FrameStatus {
		IDLE = 0,
		ACTIVE = 1
	};
	/* Handles swapchains and command buffers
	 */
	class Renderer {
		Window& window;
		Device& device;
		std::unique_ptr<SwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		FrameStatus frameStatus;
		int frameIndex;
		uint32_t imageIndex;

		void initSwapChain();
		void initCommandBuffer();
		void freeCommandBuffers();
	public:
		Renderer(Window& window, Device& device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;


		void init();
		VkCommandBuffer startFrame();
		void endFrame();
		void startRenderPass(VkCommandBuffer commandBuffer);
		void endRenderPass(VkCommandBuffer commandBuffer);

		int getFrameIndex() const { 
			assert(frameStatus == ACTIVE && "Cannot get active command buffer when frame is not active!");
			return frameIndex;
		};

		FrameStatus getFrameStatus() const { return frameStatus; };
		SwapChain& getSwapChain() const { return *swapChain; };
		VkCommandBuffer getActiveCommandBuffer() const {
			assert(frameStatus == ACTIVE && "Cannot get active command buffer when frame is not active!");
			return commandBuffers[frameIndex];
		};
		float getAspectRatio() { return swapChain->extentAspectRatio(); };

	};
}

