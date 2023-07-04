#include "CameraObject.h"

void CameraObject::update(float delta){
	MovingObject::update(delta);
	camera.setRotation(transform.translation, transform.rotation);
}
