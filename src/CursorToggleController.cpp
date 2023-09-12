#include "CursorToggleController.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"

void ic::CursorToggleController::EventCallback(int action, int mods){
	if (mods == GLFW_PRESS) {
		InputModule::cursorEnabled = !InputModule::cursorEnabled;
		glfwSetCursorPos(window, 0, 0);
		glfwSetInputMode(window, GLFW_CURSOR, (InputModule::cursorEnabled) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

	}
}
