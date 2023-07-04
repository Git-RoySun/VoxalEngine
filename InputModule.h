#pragma once
#include "KeyInput.h"
#include "MouseInput.h"
#include "window.h"

namespace ic {
	class InputModule {
		static std::unordered_map<int, KeyInput> keyMap;
		static MouseInput mouseInput;
	public:
		static void callMappedKey(GLFWwindow* window,int key, int scancode, int action, int mods);
		static void setKeyInput(int key, KeyInput input) { keyMap.insert({ key,input }); }
	};
}

