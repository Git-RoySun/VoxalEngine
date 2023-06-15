#include "VisualCore.h"

namespace vc {
	void VisualCore::start() {
		while(!window.shouldClose()) {
			glfwPollEvents();
		}
	}
}
