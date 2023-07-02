#include "IMovable.h"

#include <iostream>
#include <glm/detail/func_geometric.inl>

bool IMovable::update(){
	bool updated = false;
	if (!started) {
		started = true;
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
	started = updated;
	if(updated) {
		position += glm::normalize(step) * speed * delta.count();
		std::cout << std::format("{} {} {}", step.x, step.y, step.z) << std::endl;
	}
	last = now;
	return updated;
}

