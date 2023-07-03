#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec3.hpp>
#include <vector>

#include "CameraObject.h"
#include "InputModule.h"
#include "Object.h"
#include "VisualModule.h"

enum DIRECTION_TITLE{
	LEFT =0,
	RIGHT,
	UP,
	DOWN,
	FRONT,
	BACK,
	NUM_DIRECTION_TITLE
};

enum AXIS{
	HORIZONTAL=0,
	VERTICAL,
	DEPTH,
	NUM_AXIS
};

const int DIRECTION_PER_AXIS = 2;

const glm::vec3 DIRECTION[NUM_DIRECTION_TITLE] = {
	{-1.f,0.f,0.f},
	{1.f,0.f,0.f},
	{0.f,-1.f,0.f},
	{0.f,1.f,0.f},
	{0.f,0.f,1.f},
	{0.f,0.f,-1.f},
};

class World {
	vc::VisualModule vc{};
	ic::InputModule ic{};
	std::vector<Object> objects{};
	std::vector<CameraObject> cameras{};

	void loadWorld();
	void setup();
	void start();
public:

	void init();
};

