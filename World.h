#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec3.hpp>

enum DirectionName {
	FRONT = 0,
	BACK,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	NUM_DIRECTION_NAME
};

enum AXIS {
	DEPTH = 0,
	HORIZONTAL,
	VERTICAL,
	NUM_AXIS
};

const glm::vec3 Directions[NUM_DIRECTION_NAME] = {
		glm::vec3{0.f,0.f,1.f},
		glm::vec3{0.f,0.f,-1.f},
		glm::vec3{-1.f,0.f,0.f},
		glm::vec3{1.f,0.f,0.f},
		glm::vec3{0.f,-1.f,1.f},
		glm::vec3{0.f,1.f,1.f},
};

class World
{
};

