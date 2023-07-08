
#include <stdexcept>
#include <cstdlib>
#include <iostream>

#include "World.h"

int main(){
	try {
		World world{};
		world.init();
	}
	catch (std::exception e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
