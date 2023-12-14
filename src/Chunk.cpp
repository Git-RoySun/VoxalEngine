#include "Chunk.h"

#include "Graphic.h"
#include "Voxel.h"
#include "Observer.h"

std::shared_ptr<Chunk> Chunk::Builder::build(){
	std::shared_ptr<Chunk> res{};
	return res;
}

Chunk::Chunk(){

}

void Chunk::notifyObservers(){
	for(auto o:observers){
		o->changed = true;
	}
}




