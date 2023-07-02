#pragma once
#include <unordered_map>
#include "Command.h"
#include <functional>
#include <memory>
#include <GLFW/glfw3.h>

//TODO: Make InputComponent dependent on (per) window
namespace ic {
	class InputComponent {
		static std::unique_ptr<InputComponent> instance;
		std::unordered_map<int, Command&> commandMap{};
		bool mouseEnabled = false;
		float sensibility = 1;

		InputComponent() = default;
	public:
		~InputComponent() {};
		static InputComponent& getInstance();
		void setKeyCommand(int key, Command& command);
		void getKeyCommand(GLFWwindow* window, int key, int scancode, int action, int mods);
		void getMouseCommand(GLFWwindow* window, double xpos, double ypos);
	};
}

