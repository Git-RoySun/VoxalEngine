#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <vector>


#include "Coordinates.h"

namespace ic {

	template<typename ... T>
	class Event {
	public:
		class IHandler {
		public:
			virtual void EventCallback(T... args) = 0;
		};
	};
	typedef Event<int, int> KeyEvent;         //key, action
	typedef Event<double, double> MouseEvent; //xpos, ypos

	typedef KeyEvent::IHandler IKeyHandler;
	typedef MouseEvent::IHandler IMouseHandler;

	class InputModule {
		static std::unordered_map<int, std::vector<KeyEvent::IHandler*>> keyMap;
		static std::unordered_map <int, DIRECTION_TITLE> directionMap;

		static std::vector<MouseEvent::IHandler*> mouseListeners;
	public:
		InputModule() = delete;
		static bool cursorEnabled;
		static void sendKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void sendMouseEvent(GLFWwindow* window, double xpos, double ypos);
		static void addKeyListener(int key, KeyEvent::IHandler* listener);
		static void setDirection(int key, DIRECTION_TITLE dir) { directionMap[key] = dir; };
		static void addMouseListener(MouseEvent::IHandler* listener);
		static DIRECTION_TITLE getMappedDirection(int key) { return directionMap[key]; }
	};
}
