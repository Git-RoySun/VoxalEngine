#pragma once

#include "imgui.h"
#include "Widget.h"

namespace gm {
  template <typename T>
  class VectorInput: public Widget {
    std::string label;
    T*          content;

  public:
    VectorInput(std::string label, T* content): label{label}, content{content} {}
    void render() override { ImGui::SliderFloat3(label.c_str(), content, -20, 20); }
  };
}
