#pragma once

#include <memory>
#include "Model.h"
#include "glm/gtc/matrix_transform.hpp"

namespace vc {
	using id_t = unsigned int;
	struct Transform {
		glm::vec3 translation {};
		glm::vec3 scale {1.f, 1.f, 1.f};
		glm::vec3 rotation {};

		glm::mat4 mat4() {
			auto transform = glm::translate(glm::mat4(1.f), translation);
			return glm::scale(transform, scale);
		}
	};
	class Object {
		static id_t nextId;

		id_t instanceId;
	public:
		Object(std::unique_ptr<Model> model, Transform transform = Transform{});


		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;


		Object(Object&&) = default;
		Object& operator=(Object&&) = default;

		id_t Id() { return instanceId; };
		std::unique_ptr<Model> model;
		Transform transform;
	};
}

