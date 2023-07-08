#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec3.hpp>
#include <vector>
#include <chrono>

#include "CameraObject.h"
#include "FPMovementController.h"
#include "InputModule.h"
#include "Object.h"
#include "VisualModule.h"


class World {
	vc::VisualModule vc{};
	std::vector<Object> objects{};
	std::vector<CameraObject> cameras{};
	FPMovementController controller{nullptr};
	std::chrono::steady_clock::time_point last;

	void loadWorld();
	void setup();
	void start();
public:
	void init();
};

