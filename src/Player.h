#pragma once
#include "Camera.h"
#include "Dynamic.h"
#include "Object.h"
#include "Observer.h"
#include "Rotary.h"

class Player: public Observer, public obj::Base, public obj::Dynamic, public obj::Rotary {
	float speed = 7; //TODO add attribute system
public:
	Player(obj::Transform transform, World* world);

	void move(glm::vec3 direction) override;
	void setPosition(glm::vec3 pos) override;
	glm::vec3 getPosition() const override { return transform.position; }

	void rotate(glm::vec3 rotation) override;
	void setRotation(glm::vec3 rotation) override;
	glm::vec3 getRotation() override { return transform.rotation; }
};
