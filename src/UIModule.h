#pragma once
#include <functional>
#include <vector>

class UIModule{
	static std::vector<std::function<void()>> guis;
public:
	static void add(std::function<void()> g);
	static void render();
};

