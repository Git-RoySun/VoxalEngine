#include "VisualComponent.h"
#include "InputComponent.h"
#include "CameraObject.h"
#include "World.h"
#include <stdexcept>
#include <cstdlib>
#include <iostream>

int main(){
	try {
		CameraObject camera{};
		vc::VisualComponent vc{camera};
		ic::InputComponent& ic = ic::InputComponent::getInstance();
		ic.setKeyCommand(GLFW_KEY_D, camera.getCommand(RIGHT));
		ic.setKeyCommand(GLFW_KEY_A, camera.getCommand(LEFT));
		ic.setKeyCommand(GLFW_KEY_W, camera.getCommand(FRONT));
		ic.setKeyCommand(GLFW_KEY_S, camera.getCommand(BACK));
		auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			ic::InputComponent::getInstance().getKeyCommand(window, key, scancode, action, mods);
		};
		glfwSetKeyCallback(vc.getWindow().getGlWindow(), keyCallback);
		vc.start();
	}
	catch (std::exception e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
