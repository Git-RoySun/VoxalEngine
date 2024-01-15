#include "Graphic.h"
#include "Utils.hpp"

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
  std::cerr << pCallbackData->pMessage << "\n" << std::endl;
  return VK_FALSE;
}

static VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {
  .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
  .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
  .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
  .pfnUserCallback = debugCallback
};

#ifdef NDEBUG
bool enableValidationLayers = false;
#else
bool enableValidationLayers = true;
#endif

namespace gm {
  Module::Module() {
    window.init(&device);
  }

  Module::~Module() { vkDeviceWaitIdle(device.getVkDevice()); }

  VkInstance Module::createVkInstance() {
    addExtension(Window::getRequiredExtensions());
    volkInitialize();
    if(enableValidationLayers) {
      //Check for validation layer support
      uint32_t layerCount;
      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
      std::vector<VkLayerProperties> availableLayers(layerCount);
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
      for(const char* layerName: validationLayers) {
        bool layerFound = false;

        for(const auto& layerProperties: availableLayers) {
          if(strcmp(layerName, layerProperties.layerName) == 0) {
            layerFound = true;
            break;
          }
        }

        if(!layerFound) {
          Utils::warn("Validation layers requested, but not available!");
          enableValidationLayers = false;
          break;
        }
      }
      instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    VkApplicationInfo appInfo = {
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pEngineName = "Voxal Engine",
      .engineVersion = VK_MAKE_VERSION(0, 2, 0),
      .apiVersion = VK_API_VERSION_1_3
    };

    VkInstanceCreateInfo createInfo = {
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &appInfo,
      .enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size()),
      .ppEnabledExtensionNames = instanceExtensions.data(),
    };
    if(enableValidationLayers) {
      createInfo.enabledLayerCount   = static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();
      createInfo.pNext               = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    } else {
      createInfo.enabledLayerCount = 0;
      createInfo.pNext             = nullptr;
    }
    VK_CHECK_RESULT(vkCreateInstance(&createInfo, nullptr, &instance), "Failed to create Vulkan instance!")
    volkLoadInstance(instance);

    if(enableValidationLayers) VK_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, nullptr, &debugMessenger), "Failed to Create Debug Messenger!")

    return instance;
  }

  Module& Module::getInstance() {
    static Module singleton;
    return singleton;
  }
}
