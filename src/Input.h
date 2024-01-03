#pragma once
#include <functional>
#include <map>
#include <set>
#include <vector>

namespace im {
	template <typename... T> class Event {
	public:
		class Handler {
		public:
			virtual void onEvent(T... args) = 0;
		};
	};

	typedef Event<double, double> MouseEvent;

	class Module {
		static std::map<std::tuple<int, int, int>, std::vector<std::function<void()>>> keyMap;
		static std::set<MouseEvent::Handler*> mouseListeners;

	public:
		static Module& getInstance();

		static float sensitivity;
		static void sendKeyEvent(int key, int action, int mods, const char* keyName = nullptr, const char* actionName = nullptr, const char* modName = nullptr);
		static void sendMouseEvent(double x, double y);
		void mapKey(int key, int action, int mod, std::function<void()> f);
		void addMouseListener(MouseEvent::Handler* listener) { mouseListeners.insert(listener); }
	};
}
