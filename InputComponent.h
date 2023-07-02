#pragma once
#include <unordered_map>
#include "Command.h"
#include <functional>
#include <GLFW/glfw3.h>

namespace ic {
	class InputComponent{
		static std::unordered_map<int, Command&> commandMap;
	public:
		static void setCommand(int key, Command& command);
		static void getMap(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
}

