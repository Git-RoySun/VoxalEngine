#include "InputModule.h"
#include <iostream>
#include <glm/common.hpp>

namespace ic {
	std::unordered_map<int, std::vector<KeyEvent::IHandler*>> InputModule::keyMap = {};
	std::unordered_map <int, DIRECTION_TITLE> InputModule::directionMap = {};
	std::vector<MouseEvent::IHandler*> InputModule::mouseListeners = {};
	bool InputModule::cursorEnabled = false;

	void InputModule::sendKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto subs = keyMap.find(key);
		auto name = glfwGetKeyName(key, scancode);
		if (subs == keyMap.end() || subs->second.empty()) {
			std::cerr << "Key "<< ((name) ? name : "UNKOWN") <<" is not mapped to any action" << std::endl;
		} else {
			for(auto it:subs->second){
				it->EventCallback(key,action);
			}
		}
	};

	void InputModule::sendMouseEvent(GLFWwindow* window, double xpos, double ypos){
		if(mouseListeners.empty()){
			std::cerr << "Mouse Movement not mapped to anything" << std::endl;
		} else if(!cursorEnabled){
			for (auto it : mouseListeners) {
				if(ypos<-1000||ypos>1000){
					ypos = glm::clamp(ypos,-1000.0,1000.0);
					glfwSetCursorPos(window, xpos,ypos);
				}
				it->EventCallback(xpos,ypos);
			}
		}
	}

	void InputModule::addKeyListener(int key, KeyEvent::IHandler* listener){//need to implement function that removes
		auto subs = keyMap.find(key);
		if(subs == keyMap.end())
			subs = keyMap.insert({key, {}}).first;
		subs->second.emplace_back(listener);
	}
	void InputModule::addMouseListener(MouseEvent::IHandler* listener) {//need to implement function that removes
		mouseListeners.emplace_back(listener);
	}

}
