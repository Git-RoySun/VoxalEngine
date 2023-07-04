#include "RotateCommand.h"

#include <iostream>

#include "MovingObject.h"

void RotateCommand::rotate(double xpos, double ypos){
	target->setRotation(glm::vec3{ -ypos / target->sensibility,xpos/target->sensibility,0.f});
}
