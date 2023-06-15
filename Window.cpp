#include "Window.h"
#include <stdexcept>

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
	void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
		if (glfwCreateWindowSurface(instance, glWindow, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface!");
		}
	}
}