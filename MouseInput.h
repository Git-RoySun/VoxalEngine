#pragma once
#include <functional>

namespace ic {
	class MouseInput{
		std::function<void(double, double)> callback{};
	public:
		MouseInput() = default;
		MouseInput(std::function<void(double, double)> callback):callback(callback){};
		void move(double xpos, double ypos);
	};
}

