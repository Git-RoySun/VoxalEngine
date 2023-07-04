#pragma once
#include "Camera.h"
#include "MovingObject.h"

class CameraObject: public MovingObject{
	vc::Camera camera{};
protected:
	void update(float delta) override;
public:
	vc::Camera& getCamera() { return camera; };
};

