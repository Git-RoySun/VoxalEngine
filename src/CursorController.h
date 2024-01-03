#pragma once
#include "Input.h"
#include "Window.h"

namespace im {
	class CursorToggleController {
		GLFWwindow* window;

	public:
		CursorToggleController(GLFWwindow* window);
		void toggleCursor();
	};
}
