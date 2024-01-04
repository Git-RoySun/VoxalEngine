#pragma once
#include "volk.h"

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>

namespace im {
	class Module;
}

namespace gm {
	class Device;
	class SwapChain;

	enum {
		DEFAULT_HEIGHT = 480,
		DEFAULT_WIDTH = 854
	};

	class Window {
		static bool glfwInitialized;
		static bool initGlffw();

		uint32_t width = DEFAULT_WIDTH;
		uint32_t height = DEFAULT_HEIGHT;
		std::pair<double, double> lastCursorPos = {0, 0};

		bool resized = false;
		std::string title;

		GLFWwindow* glWindow;
		VkSurfaceKHR surface;

		Device* device;
		std::unique_ptr<SwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		bool frameInFlight = false;
		int frameIndex;
		uint32_t imageIndex;

		void initSwapChain();
		void initCommandBuffer();
		void freeCommandBuffers();

	public:
		static std::vector<const char*> getRequiredExtensions();
		Window(VkInstance instance);
		~Window();
		VkExtent2D getExtent() const { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
		GLFWwindow* getGlWindow() const { return glWindow; }
		VkSurfaceKHR getSurface() const { return surface; }
		SwapChain& getSwapChain() const { return *swapChain; }

		void init(Device* device);

		VkCommandBuffer startFrame();
		void endFrame();
		void startRenderPass(VkCommandBuffer commandBuffer);
		void endRenderPass(VkCommandBuffer commandBuffer);
		VkCommandBuffer getActiveCommandBuffer() const;

		void toggleCursor();
	};
}
