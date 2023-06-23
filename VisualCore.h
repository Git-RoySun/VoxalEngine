#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Window.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "Object.h"


namespace vc {
	class VisualCore {
		Window window{WIDTH,HEIGHT, "Window" };
		Device device{ window };
		Renderer renderer{ window,device };
		std::vector<Object> objects;

		void loadObjects();
		void initPipelineLayout();
		void initPipeline();

	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		void start();

		VisualCore();
		~VisualCore();

		VisualCore(const VisualCore&) = delete;
		VisualCore& operator=(const VisualCore&) = delete;
	};
}

