#include "FPMovementController.h"
namespace ic {
	void FPMovementController::KeyCallBack(int key, int action) {
		auto dir = ic::InputModule::getMappedDirection(key);
		auto axis = dir / NUM_AXIS;
		auto vec = DIRECTIONS[dir];

		if (action == GLFW_PRESS) {
			directionQueues[axis].emplace_back(std::move(vec));
			queueUpdate();
		}

		else if (action == GLFW_RELEASE) {
			directionQueues[axis].erase(std::remove(
				directionQueues[axis].begin(),
				directionQueues[axis].end(), vec),
				directionQueues[axis].end());

			if (directionQueues[0].size() + directionQueues[1].size() + directionQueues[2].size() == 0)
				unqueuUpdate();
		}
	}
	void FPMovementController::MouseCallBack(double xpos, double ypos) {
		rotatable->setRotation({ -ypos / sensibility,xpos / sensibility,0.f });
	}

	void FPMovementController::update(float delta) {
		glm::vec3 step =
			((directionQueues[0].empty()) ? glm::vec3{0.f} : directionQueues[0].back()) +
			((directionQueues[1].empty()) ? glm::vec3{0.f} : directionQueues[1].back()) +
			((directionQueues[2].empty()) ? glm::vec3{0.f} : directionQueues[2].back());
		float yaw = rotatable->getRotation().y;
		glm::vec3 front {sin(yaw), 0.f, cos(yaw)};
		glm::vec3 right {front.z, 0.f, -front.x};
		step = step.x * right + step.y + step.z * front;
		movable->move(step * delta);
	}
}