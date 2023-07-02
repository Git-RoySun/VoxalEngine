#include "InputComponent.h"
#include <format>
#include <iostream>


namespace ic{
	std::unordered_map<int, Command&> InputComponent::commandMap = {};

	void InputComponent::setCommand(int key, Command& command) {
		commandMap.insert({key, command});
	}

	void InputComponent::getMap(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto cmd = commandMap.find(key);
		std::string actions[3] = {
			"Release",
			"Press",
			"Repeat"
		};

		if (cmd == commandMap.end())
			std::cout << std::format("Key {} {} not mapped to any action", glfwGetKeyName(key, 0), actions[action]) << std::endl;
		else
			cmd->second.actionMap[action]();
	}

}
