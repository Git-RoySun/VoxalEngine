#include "Updatable.h"
//class functions
std::vector<Updatable*> Updatable::updateQueue = {};
std::vector<Updatable*>::iterator Updatable::add(Updatable* update){
	updateQueue.emplace_back(update);
	return --updateQueue.end();
}

void Updatable::updateAll(){
	for (auto it:updateQueue){
		it->update();
	}
}

void Updatable::queueUpdate(){
	queuePosition = Updatable::add(this);
	queued = true;
}

void Updatable::unqueuUpdate(){
	if (queued) {
		Updatable::remove(queuePosition);
		queued = false;
	}
}

