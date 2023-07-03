#pragma once
#include "Camera.h"
#include "IMovable.h"


class CameraObject: public IMovable{
	vc::Camera camera{};
public:
		bool update() override;
		vc::Camera& getVcCamera() { return camera; };
};

