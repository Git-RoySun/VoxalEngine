#include "VisualCore.h"
#include <stdexcept>
#include <cstdlib>
#include <iostream>
int main(){
	vc::VisualCore vc{};
	try{
		vc.start();
	}
	catch (std::exception e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}