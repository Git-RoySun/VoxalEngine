#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec3.hpp>
#include <vector>
#include <chrono>

#include "CameraObject.h"
#include "InputModule.h"
#include "Object.h"
#include "VisualModule.h"


class World {
	vc::VisualModule vc{};
	ic::InputModule ic{};
	std::vector<Object> objects{};
	std::vector<CameraObject> cameras{};
	std::chrono::steady_clock::time_point last;

	void loadWorld();
	void setup();
	void start();
public:

	void init();
};

