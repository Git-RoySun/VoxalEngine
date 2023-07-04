#include "Updatable.h"
//class functions
std::vector<Updatable*> Updatable::updateQueue = {};
void Updatable::updateAll(float delta){
	for (auto it:updateQueue){
		it->update(delta);
	}
}

void Updatable::queueUpdate(){
	if (!queued) {
		updateQueue.emplace_back(this);
		queued = true;
	}
}

void Updatable::unqueuUpdate(){
	if (queued) {
		for(auto it = updateQueue.begin(); it!=updateQueue.end(); ++it){
			if (*it == this) {
				updateQueue.erase(it);
				break;
			}
		}
		queued = false;
	}
}

