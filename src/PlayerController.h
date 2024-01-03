#pragma once
#include <deque>

#include "Input.h"
#include "Updatable.h"
#include "World.h"
#include "Coordinates.h"

class Player;

namespace im {
	class PlayerController: public MouseEvent::Handler, public Updatable {
		Player* target;
		std::deque<glm::vec3> directionQueues[3] = {};
		float sensibility = 1000.f;

		void addDirection(DIRECTION_TITLE dir);
		void removeDirection(DIRECTION_TITLE dir);

	public:
		PlayerController(Player* target);
		void onEvent(double y, double x) override;
		void update(float delta) override;
	};
}
