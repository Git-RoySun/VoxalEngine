#pragma once
#include "Renderer.h"

namespace gm {
  class RayTracer: public Renderer {
  public:
    RayTracer() = default;
    void render(Frame frameInfo) override;
  };
}
