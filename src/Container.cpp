#include "Container.h"

namespace gm{
	void Container::render(){
		for(auto w:widgets){
			if(w.index())//if type function<void()>
				std::get<std::function<void()>>(w)();
			else
				std::get<Widget*>(w)->render();
		}
	}

}
