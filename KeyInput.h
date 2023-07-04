#pragma once
#include <functional>
#include <iostream>

namespace ic{
	class KeyInput {
		protected:
			bool pressed = false;
			std::function<void()> press;
			std::function<void()> repeat;
			std::function<void()> release;
		public:
			KeyInput(){};
			KeyInput(std::function<void()> press, std::function<void()> repeat, std::function<void()> release):press(press), repeat(repeat), release(release) {};

			std::function<void()> actionMap[3] = {
				release,
				press,
				repeat,
			};
	};
}
