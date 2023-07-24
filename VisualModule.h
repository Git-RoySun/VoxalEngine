#pragma once
#include <memory>

#include "Window.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "ObjectRenderer.h"


namespace vc {
	struct UniformBuffer {
		glm::mat4 projectionView{1.f};
		glm::vec3 lightDirection = glm::normalize(glm::vec3{-1.f, 3.f, -1.f});
	};

	struct VisualModule {
		Window window{WIDTH,HEIGHT, "Window" };
		Device device{ window };
		Renderer renderer{ window,device };
		ObjectRenderer renderSystem{device};
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
	};
}

