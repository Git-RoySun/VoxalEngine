#pragma once
#include <vector>
#include <chrono>

#include "MoveCommand.h"
#include "IUpdatable.h"

class IMovable: public IUpdatable{
protected:
	friend class MoveCommand;
	std::chrono::steady_clock::time_point last;
	std::vector<std::vector<glm::vec3>> directionQueues {3};
	glm::vec3 position{0.f};
	float speed = 1.f;
	bool started = false;

	MoveCommand MoveCommands[NUM_DIRECTION_NAME] = {
		MoveCommand{this,FRONT},
		MoveCommand{this,BACK},
		MoveCommand{this,LEFT},
		MoveCommand{this,RIGHT},
		MoveCommand{this,UP},
		MoveCommand{this,DOWN},
	};

public:
	virtual bool update() override;
	MoveCommand& getCommand(DirectionName direction) { return MoveCommands[direction]; };
};

