#pragma once
#include "volk.h"
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>
#define WINDOW_EXTENSIONS addExtension(Window::getRequiredExtensions());

namespace gm {
  class Device;
  class SwapChain;

  enum {
    DEFAULT_HEIGHT = 480,
    DEFAULT_WIDTH  = 854
  };

  class Window {
    static bool glfwInitialized;
    static bool initGlfw();

    std::string title;

    uint32_t width  = DEFAULT_WIDTH;
    uint32_t height = DEFAULT_HEIGHT;

    std::pair<double, double> lastCursorPos = {0, 0};

    bool resized = false;

    GLFWwindow*  glWindow;
    VkSurfaceKHR surface;

    Device*                      device;
    std::unique_ptr<SwapChain>   swapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    bool     frameInFlight = false;
    int      frameIndex;
    uint32_t imageIndex;

    void initSwapChain();
    void initCommandBuffer();
    void freeCommandBuffers();

  public:
    static std::vector<const char*> getRequiredExtensions();

    Window(VkInstance instance, const std::string& title = "");
    ~Window();

    VkExtent2D getExtent() const {
      return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    }

    GLFWwindow*  getGlWindow() const { return glWindow; }
    VkSurfaceKHR getSurface() const { return surface; }
    SwapChain&   getSwapChain() const { return *swapChain; }

    void init(Device* device);

    void startRenderPass(VkCommandBuffer commandBuffer);
    void endRenderPass(VkCommandBuffer commandBuffer);

    VkCommandBuffer startFrame();
    void            endFrame();

    VkCommandBuffer getActiveCommandBuffer() const;

    void toggleCursor();
  };
}
