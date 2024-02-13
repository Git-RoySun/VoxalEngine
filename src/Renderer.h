#pragma once
#include "volk.h"
#include "Observer.h"

namespace gm {
  class Renderer {
  public:
    struct Frame {
      VkCommandBuffer commandBuffer;
      Observer*       observer;
      uint32_t        time;

      uint8_t frame;
    };

  protected:

  public:
    virtual void render(Frame frameInfo) = 0;
  };
}
