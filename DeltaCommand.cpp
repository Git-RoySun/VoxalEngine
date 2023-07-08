#include "DeltaCommand.h"
namespace ic {
	void DeltaCommand::press() {
		last = std::chrono::steady_clock::now();
		started = true;
	};
	void DeltaCommand::repeat() {
		if(started){
			auto temp = std::chrono::steady_clock::now();
			delta = temp - last;
			last = temp;
		}
	};
	void DeltaCommand::release() {
		started = false;
	};
}
