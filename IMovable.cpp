#include "IMovable.h"

#include <glm/detail/func_geometric.inl>

//Todo: Cleanup first person movement

bool IMovable::update(){
	bool updated = false;
	if (!moving) {
		moving = true;
		last = std::chrono::steady_clock::now();
	}
	auto now = std::chrono::steady_clock::now();
	std::chrono::duration<float> delta = std::chrono::duration_cast<std::chrono::duration<float>>(now - last);
	glm::vec3 step {0.0f};
	for (auto directionsQueue : directionQueues) {
		if (!directionsQueue.empty()) {
			updated = true;
			step += directionsQueue.back();
		}
	}
	moving = updated;
	if(updated) {
		float yaw = rotation.y;
		glm::vec3 front {sin(yaw), 0.f, cos(yaw)};
		glm::vec3 right {front.z, 0.f, -front.x};
		step = step.z * front + step.x * right;
		position += glm::normalize(step) * speed * delta.count();
	}
	last = now;
	updated = updated || rotated;
	rotated = false;
	return updated;
}

