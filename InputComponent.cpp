#include "InputComponent.h"
#include <format>
#include <iostream>


namespace ic{
	std::unique_ptr<InputComponent> InputComponent::instance = std::unique_ptr<InputComponent>(new InputComponent());
	InputComponent&  InputComponent::getInstance(){ return *instance; }
	void InputComponent::setKeyCommand(int key, Command& command) {
		commandMap.insert({key, command});
	}

	void InputComponent::getKeyCommand(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto cmd = commandMap.find(key);
		std::string actions[3] = {
			"Release",
			"Press",
			"Repeat"
		};
		auto name = glfwGetKeyName(key, 0);
		if (cmd == commandMap.end())
			std::cout << std::format("Key {} {} not mapped to any action",(name)?name:"UNKOWN", actions[action]) << std::endl;
		else
			cmd->second.actionMap[action]();
	}

}
