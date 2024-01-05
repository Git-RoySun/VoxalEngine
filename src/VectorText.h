#pragma once
#include <glm/gtx/string_cast.hpp>

#include "imgui.h"
#include "Widget.h"

namespace gm {
  template <typename T>
  class VectorText: public Widget {
    T* content;

  public:
    VectorText(T* content) : content{content} {}
    void render() override { return ImGui::Text(glm::to_string(*content).c_str()); }
  };
}
