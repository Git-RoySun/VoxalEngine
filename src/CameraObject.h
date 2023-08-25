#pragma once
#include "Camera.h"
#include "IMovable.h"
#include "IRotatable.h"
#include "Object.h"

class CameraObject: public Object, public IMovable, public IRotatable{
	vc::Camera camera{};
	float speed = 2.f;
public:
	glm::vec3 getRotation() const override { return transform.rotation; };
	glm::vec3 getPosition() const override { return transform.position; };

	
	void move(glm::vec3 direction) override;
	void rotate(glm::vec3 rotation) override;
	void setPosition(glm::vec3 pos) override;
	void setRotation(glm::vec3 rotation) override;
	vc::Camera* getCamera() { return &camera; };
};

