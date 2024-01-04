#include "Input.h"
#include "Utils.hpp"

#include <format>

#include "Config.h"


namespace im {
	template <typename... T> uint32_t Event<T...>::Handler::nextId = 0;

	void Module::sendKeyEvent(int key, int action, int mods, const char* keyName, const char* actionName, const char* modName) {
		const auto subs = keyMap.find(std::tuple<int, int, int>(key, action, mods));
		if(subs == keyMap.end() || subs->second.empty()) {
			Utils::warn(std::format("Unmapped Key {} {}!", (keyName) ? keyName : reinterpret_cast<const char*>(&key), (actionName) ? actionName : reinterpret_cast<const char*>(&action)));
		} else {
			for(auto it: subs->second) {
				it.onEvent();
			}
		}
	};

	void Module::sendMouseEvent(double xpos, double ypos) {
		if(!Config::cursorToggle) {
			for(const auto it: mouseListeners) {
				it.onEvent(xpos, ypos);
			}
		}
	}

	void Module::mapKey(int key, int action, int mod, const std::function<void()>& f) {
		auto p = std::tuple(key, action, mod);
		auto subs = keyMap.find(p);
		if(subs == keyMap.end() || subs->second.empty()) subs = keyMap.insert({p, {}}).first;
		subs->second.insert(KeyEvent::Handler(f));
	}

	void Module::addMouseListener(const std::function<void(double, double)>& f) {
		mouseListeners.insert(MouseEvent::Handler(f));
	}


	Module& Module::getInstance() {
		static Module singleton;
		return singleton;
	}
}
