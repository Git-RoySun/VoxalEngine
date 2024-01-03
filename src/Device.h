#pragma once
#include "volk.h"

#include <vector>
#include <memory>

namespace gm {
	class Device {
	public:
		struct QueueFamily {
			uint32_t graphicsFamily = UINT32_MAX;
			uint32_t presentFamily = UINT32_MAX;
			uint32_t computeFamily = UINT32_MAX;

			VkQueue graphicsQueue = VK_NULL_HANDLE;
			VkQueue presentQueue = VK_NULL_HANDLE;
			VkQueue computeQueue = VK_NULL_HANDLE;
			bool isComplete() const { return graphicsFamily != UINT32_MAX && presentFamily != UINT32_MAX && computeFamily != UINT32_MAX; }
		};

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
			bool isComplete() const { return !formats.empty() && !presentModes.empty(); }
		};

		class Initializer {
			VkSurfaceKHR targetSurface;
			std::vector<VkPhysicalDevice> availablePhysicalDevices{};
			std::vector<const char*> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
			void* deviceFeaturePtr = nullptr;
			std::unique_ptr<Device> device;

			//helpers
			QueueFamily findQueues(VkPhysicalDevice device) const;

			bool isDeviceSuitable(VkPhysicalDevice device);
			void pickPhysicalDevice();
			void createLogicalDevice();
			void createCommandPool();

		public:
			Initializer(VkInstance instance);
			void setTargetSurface(VkSurfaceKHR surface) { this->targetSurface = surface; }
			void* addFeature(void* addr);
			void addExtension(const char* name) { extensions.push_back(name); }
			void addExtension(std::vector<const char*> names) { extensions.insert(extensions.end(), names.begin(), names.end()); }

			std::unique_ptr<Device> init();
		};

		friend Initializer;

	private:
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		VkCommandPool commandPool;

		SwapChainSupportDetails swapChainSupport{};
		QueueFamily queues{};
		VkPhysicalDeviceProperties deviceProperties;

	public:
		Device() = default;
		~Device();

		Device(const Device&) = delete;
		Device& operator=(const Device&) = delete;
		Device(Device&&) = delete;
		Device& operator=(Device&&) = delete;

		VkDevice getVkDevice() const { return device; }
		VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
		VkCommandPool getCommandPool() const { return commandPool; }

		QueueFamily getQueueFamilies() const { return queues; }
		VkPhysicalDeviceProperties getDeviceProperties() const { return deviceProperties; }
		VkSampleCountFlagBits getMsaaSample();

		static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR targetSurface);

		VkCommandBuffer beginInstantCommands();
		void endInstantCommands(VkCommandBuffer commandBuffer);

		//TODO: Add submit to queue functions
	};
}
