#pragma once
#include <glm/vec3.hpp>

namespace obj{
	class Dynamic{
	public:
		virtual void move(glm::vec3 move) = 0;
		virtual void setPosition(glm::vec3 pos) = 0;
		virtual glm::vec3 getPosition() const = 0;
	};
}

