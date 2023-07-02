#pragma once
#include <functional>
namespace ic {
	class Command{
	protected:
		virtual void press() = 0;
		virtual void repeat() = 0;
		virtual void release() = 0;
	public:
		std::unordered_map<int, std::function<void()>> actionMap = {
				{1, [this]() { press(); }},
				{2, [this]() { repeat(); }},
				{0, [this]() { release(); }}
		};
	};
}

