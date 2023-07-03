#pragma once
#include "Camera.h"

class CameraObject{
	vc::Camera camera{};
public:
	vc::Camera& getCamera() { return camera; };
};

