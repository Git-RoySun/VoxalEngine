#pragma once
#include <glm/vec3.hpp>

class IRotatable{
public:
	virtual void rotate(glm::vec3 rotation) = 0;
};
