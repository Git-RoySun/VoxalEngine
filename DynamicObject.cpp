#include "DynamicObject.h"

void DynamicObject::move(glm::vec3 direction){
	setPosition(transform.position + direction*speed);
};

void DynamicObject::setPosition(glm::vec3 pos) {
	transform.position = pos;
};

void DynamicObject::rotate(glm::vec3 rotation){
	setRotation(transform.rotation + rotation);
};

void DynamicObject::setRotation(glm::vec3 rotation) {
	transform.rotation = rotation;
};