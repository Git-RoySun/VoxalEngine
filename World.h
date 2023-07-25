#pragma once
#include <vector>
#include <chrono>
#include <random>

#include "CameraObject.h"
#include "FPMovementController.h"
#include "Object.h"
#include "VisualContext.h"

class World {
	vc::VisualContext vc{};
	ic::FPMovementController camController{nullptr};

	std::vector<Object> objects{};
	std::vector<CameraObject> cameras{};
	
	std::chrono::steady_clock::time_point last;

	const int seed = 123456;
	bool alive = true;

	void loadObject(Object& obj);
	void loadWorld();
	void configureControl();
public:
	void setup();
	void run();
};

