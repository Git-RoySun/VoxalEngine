#include "Input.h"
#include "Utils.hpp"

#include <format>

#include "Configurations.h"


namespace im {
	std::map<std::tuple<int, int, int>, std::vector<std::function<void()>>> Module::keyMap = {};
	std::set<MouseEvent::Handler*> Module::mouseListeners = {};
	float Module::sensitivity = 100.f;

	void Module::sendKeyEvent(int key, int action, int mods, const char* keyName, const char* actionName, const char* modName) {
		const auto subs = keyMap.find(std::tuple<int, int, int>(key, action, mods));
		if(subs == keyMap.end() || subs->second.empty()) {
			Utils::warn(std::format("Unmapped Key {} {}!", (keyName) ? keyName : reinterpret_cast<const char*>(&key), (actionName) ? actionName : reinterpret_cast<const char*>(&action)));
		} else {
			for(auto it: subs->second) {
				it();
			}
		}
	};

	void Module::sendMouseEvent(double xpos, double ypos) {
		if(!Configurations::cursorToggle) {
			for(const auto it: mouseListeners) {
				it->onEvent(xpos, ypos);
			}
		}
	}

	void Module::mapKey(int key, int action, int mod, std::function<void()> f) {
		auto p = std::tuple(key, action, mod);
		auto subs = keyMap.find(p);
		if(subs == keyMap.end() || subs->second.empty()) subs = keyMap.insert({p, {}}).first;
		subs->second.emplace_back(f);
	}

	Module& Module::getInstance() {
		static Module singleton;
		return singleton;
	}
}
