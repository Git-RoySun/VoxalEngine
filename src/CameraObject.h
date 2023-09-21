#pragma once
#include "Camera.h"
#include "IMovable.h"
#include "IRotatable.h"
#include "Object.h"

namespace obj {
	class Camera : public Base, public IMovable, public IRotatable {
		vc::Camera camera{};
		float speed = 7.f;
	public:
		glm::vec3 getRotation() const override { return transform.rotation; };
		glm::vec3 getPosition() const override { return transform.position; };


		void move(glm::vec3 direction) override;
		void rotate(glm::vec3 rotation) override;
		void setPosition(glm::vec3 pos) override;
		void setRotation(glm::vec3 rotation) override;
		vc::Camera* getCamera() { return &camera; };
	};
}
