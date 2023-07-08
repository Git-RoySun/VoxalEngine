#include "RotateCommand.h"
#include "IMovable.h"
#include <iostream>
#include <format>
#include <glm/ext/scalar_constants.hpp>

void RotateCommand::rotate(double xpos,double ypos){
	target->rotation = glm::vec3 { -static_cast<float>(ypos / target->sensibility),static_cast<float>(xpos/target->sensibility), 0.0f};
	target->rotated = true;
}
