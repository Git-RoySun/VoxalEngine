#include "Window.h"

namespace vc {
	Window::Window(int h, int w, std::string name) :height{ h }, width{ w }, name{ name } {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		glWindow = glfwCreateWindow(height, width, name.c_str(), nullptr, nullptr);
	}
	Window::~Window() {
		glfwDestroyWindow(glWindow);
		glfwTerminate();
	}
}