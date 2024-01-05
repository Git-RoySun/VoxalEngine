#pragma once
#include "volk.h"
#include <memory>
#include <vector>

namespace gm {
  class DescriptorPool;
  class Device;
  class Window;
  class Widget;

  class Gui {
    std::unique_ptr<DescriptorPool> descriptorPool{};
    std::vector<Widget*> widgets{};

  public:
    Gui();
    ~Gui();
    void addWidget(Widget* w) { widgets.emplace_back(w); }
    void render(VkCommandBuffer commandBuffer);
  };
}
