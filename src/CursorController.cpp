#include "CursorController.h"

#include "Configurations.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"

im::CursorToggleController::CursorToggleController(GLFWwindow* window) : window{window} {
	Module::getInstance().mapKey(Configurations::cursorToggleKey, GLFW_PRESS, GLFW_MOD_CONTROL, [this]() { this->toggleCursor(); });
}

void im::CursorToggleController::toggleCursor() {
	Configurations::cursorToggle = !Configurations::cursorToggle;
	glfwSetCursorPos(window, 0, 0);
	glfwSetInputMode(window, GLFW_CURSOR, (Configurations::cursorToggle) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
