#pragma once
#include <glm/vec3.hpp>

namespace obj {
	class Rotary {
	public:
		virtual void rotate(glm::vec3 move) = 0;
		virtual void setRotation(glm::vec3 pos) = 0;
		virtual glm::vec3 getRotation() = 0;
	};
}

