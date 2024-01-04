#pragma once
#include <deque>

#include "Updatable.h"
#include "World.h"
#include "Coordinates.h"

class Player;


class PlayerController: public Updatable {
	Player* target;
	std::deque<glm::vec3> directionQueues[3] = {};
	float sensibility = 1000.f; //Todo should be in configs

	void addDirection(DIRECTION_TITLE dir);
	void removeDirection(DIRECTION_TITLE dir);

public:
	PlayerController(Player* target);
	void update(float delta) override;
};
