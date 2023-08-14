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
	ic::FPMovementController camController{nullptr, nullptr};

	std::vector<Object> objects{};
	//Should be replaced by a chunk map
	std::vector<CameraObject> cameras{};
	
	std::chrono::steady_clock::time_point last;

	const int seed = 1234;
	bool alive = true;

	void loadVoxel(Voxel::Instance instance);
	void loadWorld();
	void configureControl();
public:
	void setup();
	void run();
};

