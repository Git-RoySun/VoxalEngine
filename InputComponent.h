#pragma once
#include <unordered_map>
#include "KeyCommand.h"
#include <functional>
#include <memory>
#include <GLFW/glfw3.h>
#include "RotateCommand.h"

//TODO: Make InputComponent dependent on (per) window
namespace ic {
	class InputComponent {
		static std::unique_ptr<InputComponent> instance;
		std::unordered_map<int, KeyCommand&> commandMap{};
		RotateCommand* rotateCommand = nullptr;
		InputComponent() = default;
	public:
		~InputComponent() {};
		static InputComponent& getInstance();
		void setKeyCommand(int key, KeyCommand& command);
		void setMouseCommand(RotateCommand* command);
		void callKeyCommand(int key, int scancode, int action, int mods);
		void callMouseCommand(double xpos, double ypos);
	};
}

