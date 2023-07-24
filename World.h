#pragma once
#include <vector>
#include <chrono>

#include "CameraObject.h"
#include "FPMovementController.h"
#include "Object.h"
#include "VisualModule.h"
#include "Descriptor.h"

class World {
	vc::VisualModule vc{};
	std::vector<Object> objects{};
	std::vector<CameraObject> cameras{};
	ic::FPMovementController camController{nullptr};

	std::unique_ptr<vc::DescriptorPool> descriptorPool{};
	std::chrono::steady_clock::time_point last;

	void loadWorld();
	void configureControl();
public:
	void setup();
	void run();
};

