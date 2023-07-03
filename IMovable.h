#pragma once
#include <vector>
#include <chrono>

#include "MoveCommand.h"
#include "IUpdatable.h"
#include "RotateCommand.h"

class IMovable: public IUpdatable{
protected:
	friend class MoveCommand;
	friend class RotateCommand;
	std::chrono::steady_clock::time_point last;
	std::vector<std::vector<glm::vec3>> directionQueues {3};
	glm::vec3 position{0.f,0.0f,0.f};
	float speed = 1.f;
	bool moving = false;
	bool rotated = false;
	float sensibility = 1000;
	glm::vec3 rotation{0.f, 0.0f, 0.f};
	RotateCommand rotateCommand{ this };

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
	MoveCommand& getMoveCommand(DirectionName direction) { return MoveCommands[direction]; };
	RotateCommand* getRotateCommand() { return &rotateCommand; };
};

