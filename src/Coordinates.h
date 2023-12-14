#pragma once
#include <glm/vec3.hpp>

enum DIRECTION_TITLE {
	LEFT = 0,
	RIGHT,
	DOWN,
	UP,
	FRONT,
	BACK,
	NUM_DIRECTION_TITLE
};

enum AXIS {
	HORIZONTAL = 0,
	VERTICAL,
	DEPTH,
	NUM_AXIS
};

constexpr int DIRECTION_PER_AXIS = 2;

const glm::vec3 BASIS[NUM_AXIS] = {
	{1.f,0.f,0.f},
	{0.f,1.f,0.f},
	{0.f,0.f,1.f},
};

const glm::vec3 DIRECTIONS[NUM_DIRECTION_TITLE] = {
	-BASIS[HORIZONTAL],
	BASIS[HORIZONTAL],
	BASIS[VERTICAL],
	-BASIS[VERTICAL],
	BASIS[DEPTH],
	-BASIS[DEPTH],
};