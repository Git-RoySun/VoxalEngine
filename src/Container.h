#pragma once
#include <string>
#include <vector>

#include "imgui.h"
#include "Widget.h"

namespace gm {
  class Container: public Widget {
    std::string          title;
    std::vector<Widget*> widgets{};

  public:
    Container(const std::string& title, std::vector<Widget*> widgets): title{title}, widgets{std::move(widgets)} {}

    ~Container() {
      for(const auto& w: widgets) { delete w; }
      widgets.clear();
    }

    void addWidget(Widget* w) { widgets.emplace_back(w); }

    void render() override {
      ImGui::Begin(title.c_str());
      for(auto w: widgets) { w->render(); }
      ImGui::End();
    }
  };
}
