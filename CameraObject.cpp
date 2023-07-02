#include "CameraObject.h"
bool CameraObject::update(){
	bool changed = (IMovable::update() || IRotatable::update());
	if(changed)
		camera.setViewDirection(position, rotation);

	return changed;
}
