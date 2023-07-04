#pragma once
#include "Object.h"
#include "Updatable.h"
#include "MoveCommand.h"
#include <vector>

class MovingObject:public Object, public Updatable {
	std::vector<glm::vec3> directionQueues[3];
	int inputs = 0;
	float speed = 1.f;
	float sensibility = 1000.f;
	friend class MoveCommand;
	
protected:
	void update(float delta) override;
public:
	MoveCommand moveLeft { this,LEFT };
	MoveCommand moveRight { this,RIGHT };
	MoveCommand moveFront { this,FRONT };
	MoveCommand moveBack { this,BACK };
};

