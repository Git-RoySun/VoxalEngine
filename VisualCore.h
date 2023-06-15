#pragma once
#include "Window.h"
#include "Pipeline.h"

namespace vc {
	class VisualCore {
		Window window{HEIGHT, WIDTH, "Window" };
		Device device{ window };

		Pipeline Pipeline{device, "shaders/shader.vert","shaders/shader.frag", Pipeline::defaultPipelineInfo(HEIGHT, WIDTH)};

	public:
		static constexpr int HEIGHT = 800;
		static constexpr int WIDTH = 600;
		void start();
	};
}

