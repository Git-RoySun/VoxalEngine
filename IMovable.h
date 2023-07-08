#pragma once
#include <glm/vec3.hpp>

class IMovable {
	virtual void move(glm::vec3 move) = 0;
	virtual void setPosition(glm::vec3 pos) = 0;
};
