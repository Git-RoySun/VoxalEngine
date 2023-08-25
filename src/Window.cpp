#include "Window.h"
#include <stdexcept>

namespace vc {
	Window::Window(int w, int h, std::string name) :height{ h }, width{ w }, name{ name } {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		glWindow = glfwCreateWindow( width, height, name.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(glWindow, this);
		glfwSetFramebufferSizeCallback(glWindow, resize);
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

	void Window::resize(GLFWwindow* glWindow, int width, int height) {
		auto window = reinterpret_cast<Window*>(glfwGetWindowUserPointer(glWindow));
		window->resized = true;
		window->width = width;
		window->height = height;
	}
}