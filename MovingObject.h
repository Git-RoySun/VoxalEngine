#pragma once
#include "IMovable.h"
#include "IRotatable.h"
#include "Object.h"
#include "Updatable.h"
#include "World.h"

class MovingObject:public Object, public Updatable, public IMovable, public IRotatable{
	std::vector<glm::vec3> directionsQueue[NUM_AXIS];
	float speed = 1.f;
	float sensibility = 1000.f;

public:
	void update() override;
	void move(glm::vec3 direction) override;
	void rotate(glm::vec3 rotation) override;
};

