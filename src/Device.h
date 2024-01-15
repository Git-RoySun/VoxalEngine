#pragma once
#include <vector>
#include <memory>

#include "volk.h"

namespace gm {
  class Device {
  public:
    struct QueueFamily {
      uint32_t graphicsFamily = UINT32_MAX;
      uint32_t presentFamily  = UINT32_MAX;
      uint32_t computeFamily  = UINT32_MAX;

      VkQueue graphicsQueue = VK_NULL_HANDLE;
      VkQueue presentQueue  = VK_NULL_HANDLE;
      VkQueue computeQueue  = VK_NULL_HANDLE;

      bool isComplete() const {
        return
          graphicsFamily != UINT32_MAX &&
          presentFamily != UINT32_MAX &&
          computeFamily != UINT32_MAX;
      }
    };

    struct SwapChainSupportDetails {
      VkSurfaceCapabilitiesKHR        capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR>   presentModes;

      bool isComplete() const { return !formats.empty() && !presentModes.empty(); }
    };

    class Initializer;

  private:
    VkDevice         device;
    VkPhysicalDevice physicalDevice;
    VkCommandPool    commandPool;

    QueueFamily queues{};

    SwapChainSupportDetails    swapChainSupport{};
    VkPhysicalDeviceProperties deviceProperties;

  public:
    Device() = default;
    ~Device();

    Device(const Device&) noexcept   = delete;
    Device& operator=(const Device&) = delete;
    Device& operator=(Device&&)      = delete;

    Device(Device&&);

    VkDevice                       getVkDevice() const { return device; }
    VkPhysicalDevice               getPhysicalDevice() const { return physicalDevice; }
    VkCommandPool                  getCommandPool() const { return commandPool; }
    QueueFamily                    getQueueFamilies() const { return queues; }
    VkPhysicalDeviceProperties     getDeviceProperties() const { return deviceProperties; }
    VkSampleCountFlagBits          getMsaaSample();
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR targetSurface);
    VkCommandBuffer                beginInstantCommands();
    void                           endInstantCommands(VkCommandBuffer commandBuffer);

    //TODO: Add submit to queue functions
  };

  class Device::Initializer {
    Device device{};

    VkSurfaceKHR                  targetSurface;
    std::vector<VkPhysicalDevice> availablePhysicalDevices{};
    std::vector<const char*>      extensions{};
    void*                         deviceFeaturePtr = nullptr;

    //helpers
    QueueFamily findQueues(VkPhysicalDevice device) const;

    bool isDeviceSuitable(VkPhysicalDevice device);
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createCommandPool();
    void getExtensions();

    void  addExtension(const char* name) { extensions.push_back(name); }
    void  addExtension(std::vector<const char*> names) { extensions.insert(extensions.end(), names.begin(), names.end()); }
    void  setFeature(void* addr) { deviceFeaturePtr = addr; }
    void* getFeature() const { return deviceFeaturePtr; }

  public:
    Initializer(VkInstance instance, VkSurfaceKHR surface);
    Device init();
  };
}
