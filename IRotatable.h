#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "IUpdatable.h"

class IRotatable: public IUpdatable{
protected:
	glm::vec3 rotation{0.f,0.f,1.f};
	float speed = 0;
public:
	bool update() override;
};

