#include "InputModule.h"
#include <iostream>
namespace ic {
	std::unordered_map<int, KeyInput> InputModule::keyMap = {};
	void InputModule::callMappedKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto cmd = keyMap.find(key);
		auto name = glfwGetKeyName(key, scancode);
		if (cmd == keyMap.end()) {
			std::cout << "Key "<< ((name) ? name : "UNKOWN") <<" is not mapped to any action" << std::endl;
		}else {
			keyMap[key].actionMap[action]();
		}
	};
}
