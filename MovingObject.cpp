#include "MovingObject.h"

void MovingObject::update(float delta){
	glm::vec3 step{ 0.0f };
	for (auto directionsQueue : directionQueues) {
		if (!directionsQueue.empty()) {
			step += directionsQueue.back();
		}
	}
	float yaw = transform.rotation.y;
	glm::vec3 front{ sin(yaw), 0.f, cos(yaw) };
	glm::vec3 right{ front.z, 0.f, -front.x };
	step = step.z * front + step.x * right;
	transform.translation += glm::normalize(step) * speed * delta;
}

//update function