#pragma once
#include <glm/vec3.hpp>

class IMovable;

class RotateCommand{
	IMovable* target;
public:
	RotateCommand(IMovable* target) :target{ target } {}
	void rotate(double xpos, double ypos);
};

