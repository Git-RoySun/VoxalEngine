#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "Coordinates.h"

class MovingObject;
class MoveCommand{
	//this assumes keyboard input, aka
	//it is impossible to activate again without being deactivated first
	//(keys can't be pressed again without being released)
	MovingObject* target;
	DIRECTION_TITLE direction;
public:
	MoveCommand(MovingObject* target, DIRECTION_TITLE direction) :target{ target }, direction{ direction } {};
	void activate();
	void deactivate();
};

