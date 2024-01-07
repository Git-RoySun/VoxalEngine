#include <format>
#include "Input.h"
#include "Utils.hpp"
#include "Config.h"

namespace im {
	template <typename... T> uint32_t Event<T...>::Handler::nextId = 0;

	Module& Module::getInstance() {
		static Module singleton;
		return singleton;
	}

	void Module::sendKeyEvent(int key, int action, int mods, const char* keyName, const char* actionName, const char* modName) {
		const auto subs = keyMap.find(std::pair(key, action));
		if(subs == keyMap.end()) {
			Utils::warn(std::format("Unmapped Key {} {}!", (keyName) ? keyName : reinterpret_cast<const char*>(&key), (actionName) ? actionName : reinterpret_cast<const char*>(&action)));
		} else {
			for(auto& it: subs->second.first[mods]) {
				it.onEvent();
			}

			for(auto& it: subs->second.second) {
				it.onEvent();
			}
		}
	};

	void Module::mapKey(int key, int action, int mod, const std::function<void()>& f) {
		auto p = std::pair(key, action);
		auto subs = keyMap.find(p);
		if(subs == keyMap.end()) subs = keyMap.insert({p, {{}, {}}}).first;
		if(mod == 0) subs->second.second.insert(KeyEvent::Handler(f));
		else subs->second.first[mod].insert(KeyEvent::Handler(f));
	}

	void Module::sendMouseEvent(double xpos, double ypos) { if(!Config::cursorToggle) { for(const auto it: mouseListeners) { it.onEvent(xpos, ypos); } } }

	void Module::addMouseListener(const std::function<void(double, double)>& f) { mouseListeners.insert(MouseEvent::Handler(f)); }
}
