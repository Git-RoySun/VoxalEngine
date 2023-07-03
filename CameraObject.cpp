#include "CameraObject.h"

#include <iostream>
#include <glm/ext/matrix_transform.hpp>

bool CameraObject::update(){
	bool changed = IMovable::update();
	if (changed) {
		camera.setRotation(position, rotation);
		std::cout << std::format("{} {} {}", rotation.x, rotation.y, rotation.z) << std::endl;
	}
	return changed;
}
