#pragma once
#include <chrono>
#include "KeyCommand.h"

namespace ic {
	class DeltaCommand: public KeyCommand{
	protected:
		std::chrono::steady_clock::time_point last;
		std::chrono::duration<float> delta;
		bool started = false;
	public:
		virtual void press() override;
		virtual void repeat() override;
		virtual void release() override;
	};
}

