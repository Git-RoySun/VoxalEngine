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
		ic::InputComponent::setCommand(GLFW_KEY_D, camera.getCommand(RIGHT));
		ic::InputComponent::setCommand(GLFW_KEY_A, camera.getCommand(LEFT));
		ic::InputComponent::setCommand(GLFW_KEY_W, camera.getCommand(FRONT));
		ic::InputComponent::setCommand(GLFW_KEY_S, camera.getCommand(BACK));
		glfwSetKeyCallback(vc.getWindow().getGlWindow(), ic::InputComponent::getMap);
		vc.start();
	}
	catch (std::exception e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
