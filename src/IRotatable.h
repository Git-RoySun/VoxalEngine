#pragma once
#include <glm/vec3.hpp>

class IRotatable {
public:
	virtual void rotate(glm::vec3 rotation) = 0;
	virtual void setRotation(glm::vec3 rotation) = 0;
	virtual glm::vec3 getRotation() const = 0;
};
