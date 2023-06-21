#pragma once

#include <memory>
#include "Model.h"

namespace vc {
	using id_t = unsigned int;
	class Object {
		static id_t nextId;

		id_t instanceId;
	public:
		Object(std::unique_ptr<Model> model, Transform transform = {});
		

		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;


		Object(Object&&) = default;
		Object& operator=(Object&&) = default;

		id_t Id() { return instanceId; };
		std::unique_ptr<Model> model;
		Transform transform;
	};
}

