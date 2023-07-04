#include "MoveCommand.h"
#include "MovingObject.h"

void MoveCommand::activate(){
	target->directionQueues[direction/DIRECTION_PER_AXIS].emplace_back(DIRECTIONS[direction]);
	target->queueUpdate();
	++target->inputs;
}

void MoveCommand::deactivate(){
	for (auto it = target->directionQueues[direction / DIRECTION_PER_AXIS].begin(); it != target->directionQueues[direction / DIRECTION_PER_AXIS].end(); ++it) {
		if (*it == DIRECTIONS[direction]) {
			target->directionQueues[direction / DIRECTION_PER_AXIS].erase(it);
			break;
		}
	}

	if (--target->inputs == 0)
		target->unqueuUpdate();
}
