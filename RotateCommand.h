#pragma once
class MovingObject;

class RotateCommand{
	MovingObject* target;
public:
	RotateCommand(MovingObject* target) :target(target) {};
	void rotate(double xpos, double ypos);
};

