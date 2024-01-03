#include "Device.h"
#include "Utils.hpp"

#include <set>

namespace gm {
	Device::Initializer::Initializer(VkInstance instance) {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		assert(deviceCount != 0 && "Failed to find GPUs with Vulkan support!");

		availablePhysicalDevices.resize(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, availablePhysicalDevices.data());
		device = std::make_unique<Device>();
	}

	Device::~Device() {
		vkDeviceWaitIdle(device);
	}

	void* Device::Initializer::addFeature(void* addr) {
		void* res = deviceFeaturePtr;
		deviceFeaturePtr = addr;
		return res;
	}

	std::unique_ptr<Device> Device::Initializer::init() {
		pickPhysicalDevice();
		createLogicalDevice();
		createCommandPool();
		return std::move(device);
	}

	void Device::Initializer::pickPhysicalDevice() {
		for(const auto& pDevice: availablePhysicalDevices) {
			if(isDeviceSuitable(pDevice)) {
				device->physicalDevice = pDevice;
				device->swapChainSupport = Device::querySwapChainSupport(pDevice, targetSurface);
				device->queues = findQueues(pDevice);
				vkGetPhysicalDeviceProperties(pDevice, &device->deviceProperties);
				break;
			}
		}

		assert(device->physicalDevice != VK_NULL_HANDLE &&"Failed to find a suitable GPU!");
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(device->physicalDevice, &properties);
		Utils::info("Selected physical device: " + static_cast<std::string>(properties.deviceName));
	}

	void Device::Initializer::createLogicalDevice() {
		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> queueIndices = {device->queues.graphicsFamily, device->queues.computeFamily, device->queues.presentFamily,};

		float queuePriority = 1.0f;
		for(uint32_t queueFamily: queueIndices) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures enabledFeatures{.samplerAnisotropy = true, .shaderInt64 = true};

		VkDeviceCreateInfo createInfo = {.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO, .pNext = deviceFeaturePtr, .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()), .pQueueCreateInfos = queueCreateInfos.data(), .enabledExtensionCount = static_cast<uint32_t>(extensions.size()), .ppEnabledExtensionNames = extensions.data(), .pEnabledFeatures = &enabledFeatures,};

		VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{};
		if(deviceFeaturePtr) {
			physicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
			physicalDeviceFeatures2.features = enabledFeatures;
			physicalDeviceFeatures2.pNext = deviceFeaturePtr;
			createInfo.pEnabledFeatures = nullptr;
			createInfo.pNext = &physicalDeviceFeatures2;
		}

		VK_CHECK_RESULT(vkCreateDevice(device->physicalDevice, &createInfo, nullptr, &device->device), "Failed to create logical device!")

		vkGetDeviceQueue(device->device, device->queues.graphicsFamily, 0, &device->queues.graphicsQueue);
		vkGetDeviceQueue(device->device, device->queues.presentFamily, 0, &device->queues.presentQueue);
		vkGetDeviceQueue(device->device, device->queues.computeFamily, 0, &device->queues.computeQueue);
	}

	void Device::Initializer::createCommandPool() {
		const VkCommandPoolCreateInfo poolInfo = {.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO, .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, .queueFamilyIndex = device->queues.graphicsFamily,};

		VK_CHECK_RESULT(vkCreateCommandPool(device->device, &poolInfo, nullptr, &device->commandPool), "failed to create command pool!")
	}

	bool Device::Initializer::isDeviceSuitable(VkPhysicalDevice pDevice) {
		const QueueFamily indices = findQueues(pDevice);
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
		for(const auto& extension: availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		const SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pDevice, targetSurface);

		return indices.isComplete() && swapChainSupport.isComplete() && requiredExtensions.empty();
	}

	Device::QueueFamily Device::Initializer::findQueues(VkPhysicalDevice pDevice) const {
		QueueFamily queueFamily{};
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);
		std::vector<VkQueueFamilyProperties> queueIndices(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, queueIndices.data());

		int i = 0;
		for(const auto& q: queueIndices) {
			if(q.queueCount > 0) {
				if(q.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					queueFamily.graphicsFamily = i;
				}
				if(q.queueFlags & VK_QUEUE_COMPUTE_BIT) {
					queueFamily.computeFamily = i;
				}
			}
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, targetSurface, &presentSupport);
			if(q.queueCount > 0 && presentSupport) {
				queueFamily.presentFamily = i;
			}
			if(queueFamily.isComplete()) {
				break;
			}
			i++;
		}

		return queueFamily;
	}

	Device::SwapChainSupportDetails Device::querySwapChainSupport(VkPhysicalDevice pDevice, VkSurfaceKHR targetSurface) {
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, targetSurface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, targetSurface, &formatCount, nullptr);
		if(formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(pDevice, targetSurface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, targetSurface, &presentModeCount, nullptr);
		if(presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(pDevice, targetSurface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSampleCountFlagBits Device::getMsaaSample() {
		VkSampleCountFlags counts = deviceProperties.limits.framebufferColorSampleCounts & deviceProperties.limits.framebufferDepthSampleCounts;
		if(counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
		if(counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
		if(counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
		if(counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
		if(counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
		if(counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

		return VK_SAMPLE_COUNT_1_BIT;
	}

	VkCommandBuffer Device::beginInstantCommands() {
		const VkCommandBufferAllocateInfo allocInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO, .commandPool = commandPool, .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY, .commandBufferCount = 1,};

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,};

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		return commandBuffer;
	}

	void Device::endInstantCommands(VkCommandBuffer commandBuffer) {
		VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer), "Failed to end instant command");

		VkSubmitInfo submitInfo{.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO, .commandBufferCount = 1, .pCommandBuffers = &commandBuffer,};

		VkFenceCreateInfo fenceCreateInfo = {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
		VkFence fence;
		vkCreateFence(device, &fenceCreateInfo, NULL, &fence);
		VkQueue computeQueue;
		vkGetDeviceQueue(device, queues.computeFamily, 0, &computeQueue);

		vkQueueSubmit(computeQueue, 1, &submitInfo, fence);
		vkWaitForFences(device, 1, &fence, VK_TRUE, 10000000);
		vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
		vkDestroyFence(device, fence, nullptr);
	}
}
