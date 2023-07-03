#pragma once
#include <functional>

namespace ic{
	class KeyInput {
		protected:
			virtual void press() = 0;
			virtual void repeat() = 0;
			virtual void release() = 0;
		public:
			std::function<void()> actionMap[3] = {
					 [this]() { release(); },
					 [this]() { press(); },
					 [this]() { repeat(); },
			};
	};
}
