#pragma once
#include "Camera.h"
#include "DynamicObject.h"

class CameraObject: public DynamicObject{
	vc::Camera camera{};
public:
	void setPosition(glm::vec3 pos) override;
	void setRotation(glm::vec3 rotation) override;
	vc::Camera& getCamera() { return camera; };
