#pragma once
#include <memory>

#include "Window.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "RenderSystem.h"


namespace vc {
	struct VisualModule {
		Window window{WIDTH,HEIGHT, "Window" };
		Device device{ window };
		Renderer renderer{ window,device };
		RenderSystem renderSystem{device, renderer.getRenderPass()};
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
	};
}

