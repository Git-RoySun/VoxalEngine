#include "Window.h"

#include "Input.h"
#include "Device.h"
#include "SwapChain.h"
#include "Utils.hpp"

namespace gm{
	bool Window::glfwInitialized = Window::initGlffw();
	bool Window::initGlffw(){
		glfwInit();
		return true;
	}

	std::vector<const char*> Window::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		const std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		return extensions;
	}

	Window::Window(VkInstance instance) {
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		glWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		VK_CHECK_RESULT(glfwCreateWindowSurface(instance, glWindow, nullptr, &surface), "Failed to create window surface!")
		glfwSetWindowUserPointer(glWindow, this);
		glfwSetFramebufferSizeCallback(glWindow, [](GLFWwindow* glWindow, int newWidth, int newHeight) {
			Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(glWindow));
			windowPtr->width = newWidth;
			windowPtr->height = newHeight;
			windowPtr->resized = true;
		});

		glfwSetKeyCallback(glWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			const char* actionNames[3] = { "RELEASE", "PRESS", "REPEAT" };
			im::Module::sendKeyEvent(key, action, mods, glfwGetKeyName(key, scancode), actionNames[action]);
		});

		glfwSetCursorPosCallback(glWindow, [](GLFWwindow* window, double x, double y){
			if (y < -1500 || y>1500) { //TODO put 1500 as a constant
				y = std::clamp(y, -1500.0, 1500.0);
				glfwSetCursorPos(window, x, y);
			}
			im::Module::sendMouseEvent(x, y);
		});
	}

	Window::~Window(){
		freeCommandBuffers();
	}

	void Window::init(Device* device){
		this->device = device;
		initSwapChain();
		initCommandBuffer();
	}

	void Window::initSwapChain() {
		while (width == 0 || height == 0) { glfwWaitEvents(); }
		VkExtent2D extent { width, height };
		vkDeviceWaitIdle(device->getVkDevice());
		if (swapChain) {
			std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain);
			swapChain = std::make_unique<SwapChain>(device, extent , surface, oldSwapChain);
			assert(oldSwapChain->compareSwapFormat(*swapChain) && "Swap chain image format has changed!");
		}
		else
			swapChain = std::make_unique<SwapChain>(device, extent, surface);
	}

	void Window::initCommandBuffer() {
		commandBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
		VkCommandBufferAllocateInfo allocInfo = {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = device->getCommandPool(),
			.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
			.commandBufferCount = static_cast<uint32_t>(commandBuffers.size()),
		};

		VK_CHECK_RESULT(vkAllocateCommandBuffers(device->getVkDevice(), &allocInfo, commandBuffers.data()), "Failed to allocate command buffers!")
	};

	void Window::freeCommandBuffers() {
		vkFreeCommandBuffers(device->getVkDevice(), device->getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	VkCommandBuffer Window::startFrame(){
		assert(!frameInFlight && "Cannot call startFrame when frame is already active!");
		auto result = swapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			initSwapChain();
			return nullptr;
		}

		assert(result == VK_SUCCESS || result == VK_SUBOPTIMAL_KHR && "Failed to acquire chain image!");
		frameInFlight = true;
		auto commandBuffer = getActiveCommandBuffer();
		VkCommandBufferBeginInfo beginInfo = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };

		VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &beginInfo),"Failed to begin recording for command buffer")
		return commandBuffer;
	}

	void Window::endFrame() {
		assert(frameInFlight && "Cannot call endFrame when frame is not already active/started!"); 
		auto commandBuffer = getActiveCommandBuffer();
		VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer), "Failed to end recording for command buffer")

		auto result = swapChain->submitCommandBuffers(&commandBuffer, &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || resized) {
			resized = false;
			initSwapChain();
		}
		else if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to create swap chain image!");
		}

		frameInFlight = false;
		frameIndex = (frameIndex + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
	};

	void Window::startRenderPass(VkCommandBuffer commandBuffer) {
		assert(frameInFlight && "Cannot start render pass when frame is not already active/started!");
		assert(commandBuffer == getActiveCommandBuffer() && "Cannot start render pass on command buffer from another frame!");

		VkClearValue clearValues[2] = {
			{.color = {1.0f, 0.f, 1.0f, 0.0f}},
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

	void Window::endRenderPass(VkCommandBuffer commandBuffer) {
		assert(frameInFlight && "Cannot end render pass when frame is not already active/started!");
		assert(commandBuffer == getActiveCommandBuffer() && "Cannot end render pass on command buffer from another frame!");
		vkCmdEndRenderPass(commandBuffer);
	};

	VkCommandBuffer Window::getActiveCommandBuffer() const{
		assert(frameInFlight && "Cannot get active command buffer when frame is not active!");
		return commandBuffers[frameIndex];
	};
}
