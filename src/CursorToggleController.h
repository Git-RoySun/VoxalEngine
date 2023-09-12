#pragma once
#include "InputModule.h"

namespace ic{
	class CursorToggleController:public IKeyHandler {
		GLFWwindow* window;
	public:
		CursorToggleController(GLFWwindow* window) :window{ window } { EventCallback(0, GLFW_PRESS); };
		void EventCallback(int action, int mods) override;
	};
}

