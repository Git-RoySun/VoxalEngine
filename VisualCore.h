#pragma once
#include "Window.h"

namespace vc {
	class VisualCore {
		Window window{HEIGHT, WIDTH, "Window" };


	public:
		static constexpr int HEIGHT = 800;
		static constexpr int WIDTH = 600;
		void start();
	};
}

