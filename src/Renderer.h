#pragma once
#include "volk.h"
#include "Observer.h"

namespace gm {
  class Renderer {
  public:
    struct Frame {
      VkCommandBuffer commandBuffer;
      uint32_t        time;
      uint8_t         frame;
    };

  protected:
    Observer*    context;
    virtual void bindInstances(ChunkMap& chunks) = 0;

  public:
    Renderer(Observer* context) : context(context) {}
    virtual void render(Frame frameInfo) = 0;
  };
}
