#include "CameraObject.h"
void CameraObject::setPosition(glm::vec3 pos){
	DynamicObject::setPosition(pos);
	camera.setRotation(pos,transform.rotation);
}

void CameraObject::setRotation(glm::vec3 rotation){
	DynamicObject::setRotation(rotation);
	camera.setRotation(transform.position, rotation);
}
