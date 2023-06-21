#include "Object.h"

namespace vc {
	id_t Object::nextId = 0;
	Object::Object(std::unique_ptr<Model> model, Transform transform) :instanceId{ nextId++ }, model{ std::move(model) }, transform{ transform } {

	}
}