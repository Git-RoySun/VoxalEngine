#pragma once
#include <glm/vec3.hpp>

class IMovable{
public:
	virtual void move(glm::vec3 direction) = 0;
};
