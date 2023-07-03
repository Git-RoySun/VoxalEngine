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
		ic.setKeyCommand(GLFW_KEY_D, camera.getMoveCommand(RIGHT));
		ic.setKeyCommand(GLFW_KEY_A, camera.getMoveCommand(LEFT));
		ic.setKeyCommand(GLFW_KEY_W, camera.getMoveCommand(FRONT));
		ic.setKeyCommand(GLFW_KEY_S, camera.getMoveCommand(BACK));
		ic.setMouseCommand(camera.getRotateCommand());

;		auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			ic::InputComponent::getInstance().callKeyCommand(key, scancode, action, mods);
		};
		auto mouseCallback = [](GLFWwindow* window, double xpos, double ypos){
			ic::InputComponent::getInstance().callMouseCommand(xpos,ypos);
		};

		glfwSetKeyCallback(vc.getWindow().getGlWindow(), keyCallback);
		glfwSetCursorPosCallback(vc.getWindow().getGlWindow(), mouseCallback);
		glfwSetInputMode(vc.getWindow().getGlWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		vc.start();
	}
	catch (std::exception e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
