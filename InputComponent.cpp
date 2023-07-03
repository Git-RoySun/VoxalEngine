#include "InputComponent.h"
#include <format>
#include <iostream>


namespace ic{
	std::unique_ptr<InputComponent> InputComponent::instance = std::unique_ptr<InputComponent>(new InputComponent());
	InputComponent&  InputComponent::getInstance(){ return *instance; }
	void InputComponent::setKeyCommand(int key, KeyCommand& command) {
		commandMap.insert({key, command});
	}

	void InputComponent::callKeyCommand(int key, int scancode, int action, int mods) {
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
	void InputComponent::callMouseCommand(double xpos, double ypos){
		rotateCommand->rotate(xpos, ypos);
	}

	void InputComponent::setMouseCommand(RotateCommand* command){
		rotateCommand = command;
	};
}
