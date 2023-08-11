#pragma once

#include "RenderSystem.h"


namespace vc {
	/*Old render system that should be deleted*/
	class ObjectRenderer: public RenderSystem{
	public:
		void renderObjects(FrameInfo info, std::vector<Object>& objects);
	};
}

