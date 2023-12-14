#pragma once
#include <functional>
#include <string>
#include <variant>
#include <vector>

#include "Widget.h"

namespace gm{
	class Container:public Widget{
		std::string title;
		std::vector< std::variant<Widget*, std::function<void()>>> widgets{};
	public:
		Container(const std::string& title, std::vector< std::variant<Widget*, std::function<void()>>> widgets):title{title},widgets{std::move(widgets)}{}

		void addWidget(Widget* w) { widgets.emplace_back(w); }
		void addWidget(std::function<void()> f) { widgets.emplace_back(f); }
		void render() override;
	};
}

