#include "CameraObject.h"

void CameraObject::move(glm::vec3 direction) {
	setPosition(transform.position + direction * speed);
};

void CameraObject::rotate(glm::vec3 rotation) {
	setRotation(transform.rotation + rotation);
};

void CameraObject::setPosition(glm::vec3 pos){
	transform.position = pos;
	camera.setRotation(pos,transform.rotation);
}

void CameraObject::setRotation(glm::vec3 rotation){
	transform.rotation = rotation;
	camera.setRotation(transform.position, rotation);
}

