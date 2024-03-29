#pragma once
#include "volk.h"
#include <GLFW/glfw3.h>
#include <string>

namespace vc{
	class Window {
		int width;
		int height;
		bool resized = false;
		std::string name;

		GLFWwindow* glWindow;

		static void resize(GLFWwindow* glWindow, int width, int height);
	public:
		Window(int w, int h, std::string name);
		~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(glWindow); }
		bool wasResized() { return resized; }
		void resetResized() { resized = false; }

		VkExtent2D getExtent() { return {static_cast<uint32_t>(width),static_cast<uint32_t>(height) }; }
		GLFWwindow* getGlWindow() { return glWindow; };

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	};
}

