#include "PlayerController.h"

#include "Configurations.h"
#include "Dynamic.h"
#include "Rotary.h"
#include "Player.h"

namespace im{
	PlayerController::PlayerController(Player* target): target{ target } {
		auto inputs = Module::getInstance();
		inputs.addMouseListener(this);
		inputs.mapKey(Configurations::leftKey, GLFW_PRESS, 0, [this]() { this->addDirection(LEFT); });
		inputs.mapKey(Configurations::rightKey, GLFW_PRESS, 0, [this]() { this->addDirection(RIGHT); });
		inputs.mapKey(Configurations::frontKey, GLFW_PRESS, 0, [this]() { this->addDirection(FRONT); });
		inputs.mapKey(Configurations::backKey, GLFW_PRESS, 0, [this]() { this->addDirection(BACK); });

		inputs.mapKey(Configurations::leftKey, GLFW_RELEASE, 0, [this]() { this->removeDirection(LEFT); });
		inputs.mapKey(Configurations::rightKey, GLFW_RELEASE, 0, [this]() { this->removeDirection(RIGHT); });
		inputs.mapKey(Configurations::frontKey, GLFW_RELEASE, 0, [this]() { this->removeDirection(FRONT); });
		inputs.mapKey(Configurations::backKey, GLFW_RELEASE, 0, [this]() { this->removeDirection(BACK); });
	}

	void PlayerController::addDirection(DIRECTION_TITLE dir){
		auto axis = dir / DIRECTION_PER_AXIS;
		auto vec = DIRECTIONS[dir];
		directionQueues[axis].emplace_back(std::move(vec));
		queueUpdate();
	}

	void PlayerController::removeDirection(DIRECTION_TITLE dir){
		auto axis = dir / DIRECTION_PER_AXIS;
		auto vec = DIRECTIONS[dir];

		directionQueues[axis].erase(std::remove(
			directionQueues[axis].begin(),
			directionQueues[axis].end(), vec),
			directionQueues[axis].end());

		if (directionQueues[0].size() + directionQueues[1].size() + directionQueues[2].size() == 0)
			unqueuUpdate();
	}

	void PlayerController::update(float delta){
		glm::vec3 step =
			((directionQueues[0].empty()) ? glm::vec3{ 0.f } : directionQueues[0].back()) +
			((directionQueues[1].empty()) ? glm::vec3{ 0.f } : directionQueues[1].back()) +
			((directionQueues[2].empty()) ? glm::vec3{ 0.f } : directionQueues[2].back());
		static_cast<obj::Dynamic*>(target)->move(glm::normalize(step) * delta);
	}

	void PlayerController::onEvent(double y, double x){
		static_cast<Player*>(target)->setRotation({ -y / sensibility,x / sensibility,0.f });
	}

}
