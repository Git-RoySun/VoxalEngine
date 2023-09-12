#include "UIModule.h"


std::vector<std::function<void()>> UIModule::guis = std::vector<std::function<void()>>{};
void UIModule::add(std::function<void()> g){
	guis.emplace_back(g);
}

void UIModule::render(){
	for(auto g: guis){
		g();
	}
}
