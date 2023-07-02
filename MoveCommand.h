#pragma once
#include "Command.h"
#include "World.h"
class IMovable;

class MoveCommand: public ic::Command{
	bool active = false;
	IMovable* target;
	DirectionName direction;
public:
	MoveCommand(IMovable* target, DirectionName direction) : target{ target }, direction{ direction } {};
	void repeat() override {};
	void press() override;
	void release() override;
};

