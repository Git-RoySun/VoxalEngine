#pragma once
#include "Object.h"
#include "IMovable.h"
#include "IRotatable.h"

class DynamicObject:public Object, public IMovable, public IRotatable {
	float speed = 1.f;
public:
	virtual void move(glm::vec3 direction) override;
	virtual void setPosition(glm::vec3 direction) override;
	virtual void rotate(glm::vec3 rotation) override;
	virtual void setRotation(glm::vec3 rotation) override;
	glm::vec3 getRotation() { return transform.rotation; };
	glm::vec3 getPosition() { return transform.position; };
};

