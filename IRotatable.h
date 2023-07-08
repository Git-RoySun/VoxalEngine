#pragma once
#include <glm/vec3.hpp>

class IRotatable {
	virtual void rotate(glm::vec3 rotation) = 0;
	virtual void setRotation(glm::vec3 rotation) = 0;
};
