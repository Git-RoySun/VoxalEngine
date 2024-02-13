#pragma once
#include "Device.h"
#include "Window.h"

#ifdef NDEBUG
#define vl {}
#else
#define vl { "VK_LAYER_KHRONOS_validation" }
#endif

#define WINDOW gm::Module::getInstance().getWindow()
#define DEVICE gm::Module::getInstance().getDevice()

namespace gm {
  class Module {
    VkInstance               instance = createVkInstance();
    VkDebugUtilsMessengerEXT debugMessenger;
    std::vector<const char*> validationLayers = vl;
    std::vector<const char*> instanceExtensions;
    VkInstance               createVkInstance();

    Window window{instance};
    Device device{Device::Initializer(instance, window.getSurface()).init()};

    Module();
    ~Module();

  public:
    static Module& getInstance();

    void addExtension(const char* name) { instanceExtensions.push_back(name); }

    void addExtension(std::vector<const char*> names) {
      for(auto it: names) {
        addExtension(it);
      }
    }

    VkInstance getVkInstance() { return instance; }

    Window& getWindow() { return window; }
    Device& getDevice() { return device; }
  };
}
