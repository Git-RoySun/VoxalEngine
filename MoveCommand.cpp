#include "MoveCommand.h"
#include "IMovable.h"
#include <iostream>

void MoveCommand::press(){
	target->directionQueues[direction/2].emplace_back(Directions[direction]);
	active = true;
}

void MoveCommand::release() {
	for (auto it = target->directionQueues[direction / 2].begin();it!=target->directionQueues[direction / 2].end();++it){
		if(*it == Directions[direction]){
			target->directionQueues[direction / 2].erase(it);
			break;
		}
	}
	active = false;
}