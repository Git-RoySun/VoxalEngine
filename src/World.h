#pragma once
#include <vector>
#include <chrono>

#include "CameraObject.h"
#include "ChunkLoader.h"
#include "CursorToggleController.h"
#include "FPMovementController.h"
#include "VisualContext.h"

class World {
	vc::VisualContext vc{};
	ic::FPMovementController camController{nullptr, nullptr};
	ic::CursorToggleController cursorController{vc.getWindow().getGlWindow()};
	
	ChunkLoader loader{ vc };
	std::vector<obj::Camera> cameras{};
	
	std::chrono::steady_clock::time_point last;

	const int seed = 3241561;
	
	void loadWorld();
	void configureControl();
public:
	void setup();
	void run();
};

