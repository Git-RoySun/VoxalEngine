#include "CameraObject.h"

void CameraObject::update(float delta){
	MovingObject::update(delta);
	camera.setRotation(transform.translation, transform.rotation);
}
void CameraObject::setRotation(glm::vec3 rotation){
	MovingObject::setRotation(rotation);
	camera.setRotation(transform.translation, transform.rotation);
}
