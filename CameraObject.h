#pragma once
#include "Camera.h"
#include "IMovable.h"
#include "IRotatable.h"


class CameraObject: public IMovable, public IRotatable{
	vc::Camera camera{};
public:
		bool update() override;
		vc::Camera& getVcCamera() { return camera; };
};

